#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkProxyFactory>
#include <QPushButton>
#include <QSplashScreen>
#include <QTextStream>

#include <QtTest/QTest>

#include "QMConsole.h"
#include "QMDecorateDir.h"
#include "QMSystem.h"
#include "QMWidgetsHost.h"

#include "QBreakpadHandler.h"

#include "extensionsystem/pluginmanager.h"
#include "extensionsystem/pluginspec.h"

#include "singleapplication.h"

#define USE_NATIVE_MESSAGEBOX

using namespace ExtensionSystem;

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

static const SingleApplication::Options opts =
    SingleApplication::ExcludeAppPath | SingleApplication::ExcludeAppVersion | SingleApplication::SecondaryNotification;


enum { OptionIndent = 4, DescriptionIndent = 34 };

const char corePluginNameC[] = "Core";
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
    msgbox.setWindowTitle(qAppName());
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
    qmCon->MsgBox(nullptr, QMConsole::Critical, qAppName(), t);
#endif
    QMOs::exitApp(-1);
}

static inline void displayHelpText(const QString &t) {
#ifndef USE_NATIVE_MESSAGEBOX
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Information);
    msgbox.setWindowTitle(qAppName());
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
    qmCon->MsgBox(nullptr, QMConsole::Information, qAppName(), t);
#endif

    QMOs::exitApp(0);
}

static inline void printVersion(const PluginSpec *coreplugin) {
    QString version;
    QTextStream str(&version);
    str << '\n' << qAppName() << ' ' << coreplugin->version() << " based on Qt " << qVersion() << "\n\n";
    PluginManager::formatPluginVersions(str);
    str << '\n' << coreplugin->copyright() << '\n';
    displayHelpText(version);
}

static inline void printHelp() {
    QString help;
    QTextStream str(&help);

    str << "Usage: " << qAppName() << fixedOptionsC;
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
    return QCoreApplication::translate("Application", "Failed to load core: %1!").arg(why);
}

static inline QStringList getPluginPaths() {
    QStringList rc;
    QFileInfo info(QMFs::binaryPath() + "/../lib/" + qAppName() + "/plugins");
    if (info.isDir()) {
        rc += info.absoluteFilePath();
    }
    return rc;
}

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
    a.setOrganizationDomain("org.ChorusKit");

    QMWidgetsHost host;

    // Add qslib plugin dir
    QApplication::addLibraryPath(host.libDir() + "/QsLib/plugins");

    // Load decorators
    QMDecorateDir dummyDec;
    dummyDec.load(QString("%1/%2/%2.res.json").arg(host.shareDir(), "qtdummy")); // qtdummy

    // Parse command line
    bool allowRoot = false;
    bool showHelp = false;
    QStringList customPluginPaths;
    QStringList arguments = a.arguments(); // adapted arguments list is passed to plugin manager later
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

    // Root privilege detection
    if (QMOs::isUserRoot() && !allowRoot) {
        if (showHelp) {
            printHelp();
            return 0;
        }

        QString msg = QCoreApplication::tr("You're trying to start %1 as the %2, which is "
                                           "extremely dangerous and therefore strongly not recommended.")
                          .arg(qAppName(), QMOs::rootUserName());
        qmCon->MsgBox(nullptr, QMCoreConsole::Warning, qAppName(), msg);
        return 0;
    }

    // Prepare to start application
    if (!host.createDataAndTempDirs()) {
        return -1;
    }

    QSplashScreen splash(QPixmap(":/svsplash.png"));
    splash.show();

    g_splash = &splash;
    a.setProperty("__choruskit_init_splash__", QVariant::fromValue(&splash));

#ifdef CONFIG_ENABLE_BREAKPAD
    // Prepare to load plugins
    QBreakpadInstance.setDumpPath(qmHost->appDataDir() + "/crashes");
#endif

    // Make sure we honor the system's proxy settings
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // Don't show plugin manager debug info
    QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, qmHost->appDataDir());
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, qmHost->appDataDir());

    PluginManager pluginManager;
    pluginManager.setPluginIID(QString("org.ChorusKit.%1.Plugin").arg(qAppName()));

    auto settings =
        new QSettings(QSettings::IniFormat, QSettings::SystemScope, a.organizationDomain(), a.applicationName());
    pluginManager.setGlobalSettings(settings);

    QStringList pluginPaths = getPluginPaths() + customPluginPaths;
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
    PluginSpec *coreplugin = 0;
    for (auto spec : qAsConst(plugins)) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }

    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason =
            QCoreApplication::translate("Application", "Could not find Core plugin in %1!").arg(nativePaths);
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }

    if (!coreplugin->isEffectivelyEnabled()) {
        const QString reason = QCoreApplication::translate("Application", "Core plugin is disabled.");
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
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
        qInfo() << "apploader: primary instance already running. PID:" << single.primaryPid();

        if (!vstiAddr.isEmpty()) {
            displayError(QCoreApplication::tr("Please close the running application!"));
            return 0;
        }

        // This eventually needs moved into the NotepadNextApplication to keep
        // sending/receiving logic in the same place
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::WriteOnly);

        stream << PluginManager::serializedArguments();
        single.sendMessage(buffer);

        qInfo() << "apploader: secondary instance closing...";

        return 0;
    } else {
        qInfo() << "apploader: primary instance initializing...";
    }

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
        qDebug().noquote().nospace() << "apploader: remote message from " << instanceId << ", " << msg;
        pluginManager.remoteArguments(msg, nullptr);
    });

    // shutdown plugin manager on the exit
    QObject::connect(&a, &QApplication::aboutToQuit, &pluginManager, &PluginManager::shutdown);

    // Load app decorator (If exists)
    QMDecorateDir appDec;
    appDec.load(QString("%1/%2/%2.res.json").arg(host.shareDir(), qAppName())); // app

    return a.exec();
}
