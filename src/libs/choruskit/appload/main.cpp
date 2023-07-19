#include <QApplication>
#include <QDir>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QNetworkProxyFactory>
#include <QProcess>
#include <QTextStream>

#include "QMAppExtension.h"
#include "QMConsole.h"
#include "QMCss.h"
#include "QMDecoratorV2.h"
#include "QMSystem.h"

#include "QBreakpadHandler.h"

#include "extensionsystem/pluginmanager.h"
#include "extensionsystem/pluginspec.h"

#include "singleapplication.h"

#include "config/loadconfig.h"
#include "splash/SplashScreen.h"

#include "CoreApi/ILoader.h"


Q_LOGGING_CATEGORY(loaderLog, "apploader")

#define USE_NATIVE_MESSAGEBOX

using namespace ExtensionSystem;

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

static const SingleApplication::Options opts =
    SingleApplication::ExcludeAppPath | SingleApplication::ExcludeAppVersion | SingleApplication::SecondaryNotification;


enum { OptionIndent = 4, DescriptionIndent = 34 };

static const char fixedOptionsC[] = " [options]... [files]...\n";

static const char HELP_OPTION1[] = "-h";
static const char HELP_OPTION2[] = "--help";
static const char VERSION_OPTION1[] = "-v";
static const char VERSION_OPTION2[] = "--version";
static const char ALLOW_ROOT_OPTION[] = "--allow-root";
static const char PLUGIN_PATH_OPTION[] = "--plugin-path";

static QSplashScreen *g_splash = nullptr;

// Format as <pre> HTML
static inline QString toHtml(const QString &t) {
    QString res = t;
    res.replace(QLatin1Char('&'), QLatin1String("&amp;"));
    res.replace(QLatin1Char('<'), QLatin1String("&lt;"));
    res.replace(QLatin1Char('>'), QLatin1String("&gt;"));
    res.prepend(QLatin1String("<html><pre>"));
    res.append(QLatin1String("</pre></html>"));
    return res;
}

static inline void indent(QTextStream &str, int indent) {
    const QChar blank = QLatin1Char(' ');
    for (int i = 0; i < indent; i++)
        str << blank;
}

static inline void formatOption(QTextStream &str, const QStringList &opts, const QString &parm,
                                const QString &description) {
    QString opt = opts.join('/');
    int remainingIndent = DescriptionIndent - OptionIndent - opt.size();
    indent(str, OptionIndent);
    str << opt;
    if (!parm.isEmpty()) {
        str << " <" << parm << '>';
        remainingIndent -= 3 + parm.size();
    }
    if (remainingIndent >= 1) {
        indent(str, remainingIndent);
    } else {
        str << '\n';
        indent(str, DescriptionIndent);
    }
    str << description << '\n';
}

static inline void displayError(const QString &t) {
#ifndef USE_NATIVE_MESSAGEBOX
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.setWindowTitle(qApp->applicationName());
    msgbox.setText(toHtml(t));
    msgbox.show();
    if (g_splash) {
        g_splash->finish(&msgbox);
    }
    msgbox.exec();
#else
    if (g_splash) {
        g_splash->close();
    }
    qmCon->MsgBox(nullptr, QMConsole::Critical, qApp->applicationName(), t);
#endif
    QMOs::exitApp(-1);
}

static inline void displayHelpText(const QString &t) {
#ifndef USE_NATIVE_MESSAGEBOX
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Information);
    msgbox.setWindowTitle(qApp->applicationName());
    msgbox.setText(toHtml(t));
    msgbox.show();
    if (g_splash) {
        g_splash->finish(&msgbox);
    }
    msgbox.exec();
#else
    if (g_splash) {
        g_splash->close();
    }
    qmCon->MsgBox(nullptr, QMConsole::Information, qApp->applicationName(), t);
#endif

    QMOs::exitApp(0);
}

static inline void printVersion(const PluginSpec *coreplugin) {
    QString version;
    QTextStream str(&version);
    str << '\n' << qApp->applicationName() << ' ' << coreplugin->version() << " based on Qt " << qVersion() << "\n\n";
    PluginManager::formatPluginVersions(str);
    str << '\n' << coreplugin->copyright() << '\n';
    displayHelpText(version);
}

static inline void printHelp() {
    QString help;
    QTextStream str(&help);

    str << "Usage: " << qApp->applicationName() << fixedOptionsC;
    formatOption(str, {HELP_OPTION1, HELP_OPTION2}, {}, "Display this help");
    formatOption(str, {VERSION_OPTION1, VERSION_OPTION2}, {}, "Display application version");
    formatOption(str,
                 {
                     ALLOW_ROOT_OPTION,
                 },
                 {}, "Allow running with root privilege");
    formatOption(str,
                 {
                     PLUGIN_PATH_OPTION,
                 },
                 {}, "Add a custom search path for plugins");
    PluginManager::formatOptions(str, OptionIndent, DescriptionIndent);
    if (PluginManager::instance()) {
        PluginManager::PluginManager::formatPluginOptions(str, OptionIndent, DescriptionIndent);
    }
    displayHelpText(help);
}

static inline QString msgCoreLoadFailure(const QString &why) {
    return QCoreApplication::translate("Application", "Failed to load core: %1").arg(why);
}

class Restarter {
public:
    Restarter(int argc, char *argv[]) {
        Q_UNUSED(argc)
        m_executable = QString::fromLocal8Bit(argv[0]);
        m_workingPath = QDir::currentPath();
    }

    void setArguments(const QStringList &args) {
        m_args = args;
    }

    QString executable() const {
        return m_executable;
    }
    QStringList arguments() const {
        return m_args;
    }
    QString workingPath() const {
        return m_workingPath;
    }

    int restartOrExit(int exitCode) {
        return qApp->property("restart").toBool() ? restart(exitCode) : exitCode;
    }

    int restart(int exitCode) {
        QProcess::startDetached(m_executable, m_args, m_workingPath);
        return exitCode;
    }

private:
    QString m_executable;
    QStringList m_args;
    QString m_workingPath;
};

int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Get application information from env
    QString hint;
    QString vstiAddr;
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_NAME_HINT")).isEmpty()) {
        a.setApplicationName(hint);
    }
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_VERSION_HINT")).isEmpty()) {
        a.setApplicationVersion(hint);
    }
    if (!(hint = qEnvironmentVariable("CHORUSKIT_VSTI_BLOCK_ADDRESS")).isEmpty()) {
        vstiAddr = hint;
    }
    a.setOrganizationName("ChorusKit");
    a.setOrganizationDomain("ChorusKit");

    QMAppExtension host;

    Q_ASSERT(qApp->applicationName() == a.applicationName());
    QString corePluginNameC = "Core";
    QString pluginIID = QString("org.ChorusKit.%1.Plugin").arg(qApp->applicationName());
    QString splashImagePath;
    QImage splashImage;
    QSize splashSize;
    QStringList resourcesFiles; // Unused

    // Load configuration
    LoadConfig configFile;
    QString configDir = host.appShareDir();
    if (configFile.load(QString("%1/config.json").arg(configDir))) {
        if (!configFile.pluginIID.isEmpty()) {
            pluginIID = configFile.pluginIID;
        }
        if (!configFile.splashImage.isEmpty()) {
            QString path = configFile.splashImage;
            if (QDir::isRelativePath(path)) {
                path = configDir + "/" + path;
            }
            splashImagePath = path;
        }
        if (configFile.splashSize.size() == 2) {
            splashSize = QSize(configFile.splashSize.front(), configFile.splashSize.back());
        }
        if (!configFile.coreName.isEmpty()) {
            corePluginNameC = configFile.coreName;
        }
        for (const auto &file : qAsConst(configFile.resourceFiles)) {
            if (QDir::isRelativePath(file)) {
                resourcesFiles.append(configDir + "/" + file);
            } else {
                resourcesFiles.append(file);
            }
        }
    }

    if (splashImage.isNull()) {
        // splashImage = QPixmap(":/yqzhishen.png");
        splashImage = QImage(":/A60.jpg");
    }

    if (splashSize.isEmpty()) {
        splashSize = splashImage.size();
    }

    // Parse command line
    bool allowRoot = false;
    bool showHelp = false;
    QStringList customPluginPaths;
    QStringList arguments = a.arguments(); // adapted arguments list is passed to plugin manager later
    {
        QMutableStringListIterator it(arguments);
        while (it.hasNext()) {
            const QString &arg = it.next();
            if (arg == QLatin1String(ALLOW_ROOT_OPTION)) {
                it.remove();
                allowRoot = true;
            } else if (arg == QLatin1String(PLUGIN_PATH_OPTION)) {
                it.remove();
                if (it.hasNext()) {
                    customPluginPaths << it.next();
                    it.remove();
                }
            } else if (arg == HELP_OPTION1 || arg == HELP_OPTION2) {
                showHelp = true;
            } else if (arg.startsWith('-')) {
                if (it.hasNext()) {
                    it.next();
                }
            }
        }
    }

#define TEST_HELP                                                                                                      \
    if (showHelp) {                                                                                                    \
        printHelp();                                                                                                   \
        return 0;                                                                                                      \
    }

    // Root privilege detection
    if (QMOs::isUserRoot() && !allowRoot) {
        TEST_HELP

        QString msg =
            QCoreApplication::translate("Application", "You're trying to start %1 as the %2, which is "
                                                       "extremely dangerous and therefore strongly not recommended.")
                .arg(qApp->applicationName(), QMOs::rootUserName());
        qmCon->MsgBox(nullptr, QMCoreConsole::Warning, qApp->applicationName(), msg);
        return 0;
    }

    // Prepare to start application (Skip if show help)
    if (!showHelp && !host.createDataAndTempDirs()) {
        return -1;
    }

    // Unique loader
    Core::ILoader loader;

    QString userSettingPath = host.appDataDir();
    QString systemSettingPath = host.appShareDir();

    loader.setSettingsPath(QSettings::UserScope,
                           QString("%1/%2.settings.json").arg(userSettingPath, qApp->applicationName()));
    loader.setSettingsPath(QSettings::SystemScope,
                           QString("%1/%2.settings.json").arg(systemSettingPath, qApp->applicationName()));

#ifdef CONFIG_ENABLE_BREAKPAD
    // Prepare to load plugins
    QBreakpadInstance.setDumpPath(qmHost->appDataDir() + "/crashes");
#endif

    // Make sure we honor the system's proxy settings
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // Don't show plugin manager debug info
    QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, userSettingPath);
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, systemSettingPath);

    PluginManager pluginManager;
    pluginManager.setPluginIID(pluginIID);

    pluginManager.setSettings(new QSettings(QString("%1/%2.plugins.ini").arg(userSettingPath, qApp->applicationName()),
                                            QSettings::IniFormat));
    pluginManager.setGlobalSettings(new QSettings(
        QString("%1/%2.plugins.ini").arg(systemSettingPath, qApp->applicationName()), QSettings::IniFormat));

    // Restore language and themes
    {
        auto settings = PluginManager::settings();
        settings->beginGroup("Preferences");
        auto value = settings->value("Translation");
        if (value.type() == QVariant::String) {
            qIDec->setLocale(value.toString());
        }

        value = settings->value("Theme");
        if (value.type() == QVariant::String) {
            qIDec->setTheme(value.toString());
        }
        settings->endGroup();
    }

    SplashScreen splash;
    g_splash = &splash;

    auto ratio = splash.screen()->logicalDotsPerInch() / QMOs::unitDpi() * 0.8;
    if (configFile.resizable) {
        splashSize *= ratio;
    }

    Core::InitialRoutine initialRoutine(&splash);

    // Add initial routine to loader object pool
    loader.addObject("choruskit_init_routine", &initialRoutine);
    QObject::connect(&initialRoutine, &Core::InitialRoutine::done, [&] {
        loader.removeObject(&initialRoutine); //
    });

    QPixmap pixmap;
    if (splashImagePath.endsWith(".svg", Qt::CaseInsensitive)) {
        pixmap = QIcon(splashImagePath).pixmap(splashSize);
    } else {
        pixmap = QPixmap::fromImage(splashImage.scaled(splashSize));
    }
    splash.setPixmap(pixmap);

    for (auto it = configFile.splashSettings.texts.begin(); it != configFile.splashSettings.texts.end(); ++it) {
        const auto &item = it.value();
        SplashScreen::Attribute attr;
        attr.pos = item.pos.size() == 2 ? QPoint(item.pos[0], item.pos[1]) : attr.pos;
        attr.anchor = item.anchor.size() == 2 ? qMakePair(item.anchor[0], item.anchor[1]) : attr.anchor;
        attr.fontSize = item.fontSize > 0 ? item.fontSize : attr.fontSize;
        attr.fontColor = QMCss::CssStringToColor(item.fontColor);
        attr.maxWidth = item.maxWidth > 0 ? item.maxWidth : attr.maxWidth;
        attr.text = item.text;

        if (configFile.resizable) {
            attr.pos *= ratio;
            attr.anchor.first *= ratio;
            attr.anchor.second *= ratio;
            attr.fontSize *= ratio;
            attr.maxWidth *= ratio;
        }

        splash.setTextAttribute(it.key(), attr);
    }

    splash.show();

    // Don't know why drawing text blocks so much time, so we show splash first and then show texts
    splash.showTexts();

    // Update loader text
    splash.showMessage(QCoreApplication::translate("Application", "Searching plugins..."));

    QStringList pluginPaths = [&]() {
        QStringList rc;
        QFileInfo info(host.appPluginsDir());
        if (info.isDir()) {
            rc += info.absoluteFilePath();
        }
        return rc;
    }() + customPluginPaths;

    pluginManager.setPluginPaths(pluginPaths);

    // Parse arguments again
    QMap<QString, QString> foundAppOptions;
    if (arguments.size() > 1) {
        QMap<QString, bool> appOptions;
        appOptions.insert(QLatin1String(HELP_OPTION1), false);
        appOptions.insert(QLatin1String(HELP_OPTION2), false);
        appOptions.insert(QLatin1String(VERSION_OPTION1), false);
        appOptions.insert(QLatin1String(VERSION_OPTION2), false);
        QString errorMessage;
        if (!PluginManager::parseOptions(arguments, appOptions, &foundAppOptions, &errorMessage)) {
            displayError(errorMessage);
            printHelp();
            return -1;
        }
    }

    // Load plugins
    const auto plugins = PluginManager::plugins();
    PluginSpec *coreplugin = nullptr;
    for (auto spec : qAsConst(plugins)) {
        if (spec->name() == corePluginNameC) {
            coreplugin = spec;
            break;
        }
    }

    if (!coreplugin) {
        TEST_HELP
        // QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin!");
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }

    if (!coreplugin->isEffectivelyEnabled()) {
        TEST_HELP
        const QString reason = QCoreApplication::translate("Application", "Core plugin is disabled.");
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
        TEST_HELP
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }
    if (foundAppOptions.contains(QLatin1String(VERSION_OPTION1)) || foundAppOptions.contains(VERSION_OPTION2)) {
        printVersion(coreplugin);
        return 0;
    }
    if (foundAppOptions.contains(QLatin1String(HELP_OPTION1)) ||
        foundAppOptions.contains(QLatin1String(HELP_OPTION2))) {
        printHelp();
        return 0;
    }

    // Init single handle
    SingleApplication single(qApp, true, opts);
    if (!single.isPrimary()) {
        qCDebug(loaderLog) << "primary instance already running. PID:" << single.primaryPid();

        if (!vstiAddr.isEmpty()) {
            displayError(QCoreApplication::translate("Application", "Please close the running application!"));
            return 0;
        }

        // This eventually needs moved into the NotepadNextApplication to keep
        // sending/receiving logic in the same place
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::WriteOnly);

        stream << PluginManager::serializedArguments();
        single.sendMessage(buffer);

        qCDebug(loaderLog) << "secondary instance closing...";

        return 0;
    } else {
        qCDebug(loaderLog) << "primary instance initializing...";
    }

    // Update loader text
    splash.showMessage(QCoreApplication::translate("Application", "Loading plugins..."));

    PluginManager::loadPlugins();
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }

    // Set up remote arguments.
    QObject::connect(&single, &SingleApplication::receivedMessage, [&](quint32 instanceId, QByteArray message) {
        QDataStream stream(&message, QIODevice::ReadOnly);
        QString msg;
        stream >> msg;
        qCDebug(loaderLog).noquote().nospace() << " remote message from " << instanceId << ", " << msg;
        pluginManager.remoteArguments(msg, nullptr);
    });

    // shutdown plugin manager on the exit
    QObject::connect(&a, &QApplication::aboutToQuit, &pluginManager, &PluginManager::shutdown);

    return Restarter(argc, argv).restartOrExit(a.exec());
}
