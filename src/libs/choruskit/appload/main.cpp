#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QPushButton>
#include <QSplashScreen>
#include <QTextStream>

#include "QMConsole.h"
#include "QMSystem.h"
#include "QMWidgetsHost.h"
#include "QsCoreConfig.h"
#include "QsStartInfo.h"

#include "QBreakpadHandler.h"
#include "Windows/PlainWindow.h"

#include "extensionsystem/pluginmanager.h"

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

enum { OptionIndent = 4, DescriptionIndent = 34 };

static const char fixedOptionsC[] = " [options]... [files]...\n";

static const char HELP_OPTION1[] = "-h";
static const char HELP_OPTION2[] = "--help";
static const char VERSION_OPTION1[] = "-v";
static const char VERSION_OPTION2[] = "--version";
static const char ALLOW_ROOT_OPTION[] = "--allow-root";
static const char RESET_CONFIG_OPTION[] = "--reset-config";
static const char PLUGIN_PATH_OPTION[] = "--plugin-path";

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

[[maybe_unused]] static inline void displayError(const QString &t) {
    qmCon->MsgBox(nullptr, QMConsole::Critical, qAppName(), t);
    QMOs::exitApp(0);
}

static inline void displayHelpText(const QString &t) {
    qmCon->MsgBox(nullptr, QMConsole::Information, qAppName(), t);
    QMOs::exitApp(0);
}

static inline void printVersion(/*const PluginSpec *coreplugin*/) {
    QString version;
    QTextStream str(&version);
    str << '\n'
        << qAppName() /*<< ' ' << coreplugin->version()*/ << " based on Qt " << qVersion()
        << "\n\n";
    // PluginManager::formatPluginVersions(str);
    // str << '\n' << coreplugin->copyright() << '\n';
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
                     RESET_CONFIG_OPTION,
                 },
                 {}, "Generate or reset configuration file");
    formatOption(str,
                 {
                     PLUGIN_PATH_OPTION,
                 },
                 {}, "Add a custom search path for plugins");
    // ExtensionSystem::PluginManager::formatOptions(str, OptionIndent, DescriptionIndent);
    // ExtensionSystem::PluginManager::PluginManager::formatPluginOptions(str, OptionIndent,
    // DescriptionIndent);
    displayHelpText(help);
}

int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QString coreName = "coreplugin";

    // Get application information from env
    QString hint;
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_NAME_HINT")).isEmpty()) {
        qDebug() << "loader:"
                 << "App Name" << hint;
        a.setApplicationName(hint);
    }
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_VERSION_HINT")).isEmpty()) {
        qDebug() << "loader:"
                 << "App Version" << hint;
        a.setApplicationVersion(hint);
    }
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_ORG_HINT")).isEmpty()) {
        qDebug() << "loader:"
                 << "App Organization" << hint;
        a.setOrganizationName(hint);
    }
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_DOMAIN_HINT")).isEmpty()) {
        qDebug() << "loader:"
                 << "App Domain" << hint;
        a.setOrganizationDomain(hint);
    }
    if (!(hint = qEnvironmentVariable("CHORUSKIT_APP_CORE_HINT")).isEmpty()) {
        qDebug() << "loader:"
                 << "App Core" << hint;
        coreName = hint;
    }

    QFont f("Microsoft YaHei UI");
    f.setPixelSize(15);
    a.setFont(f);

    QMWidgetsHost host; // QtMedium host
    QsStartInfo info;   // QsLib host

    // Parse command line
    bool hasHelp = false;
    bool hasVersion = false;
    QStringList arguments =
        a.arguments(); // adapted arguments list is passed to plugin manager later
    QMutableStringListIterator it(arguments);
    while (it.hasNext()) {
        const QString &arg = it.next();
        if (arg == QLatin1String(HELP_OPTION1) || arg == QLatin1String(HELP_OPTION2)) {
            hasHelp = true;
            it.remove();
        } else if (arg == QLatin1String(VERSION_OPTION1) || arg == QLatin1String(VERSION_OPTION2)) {
            hasVersion = true;
            it.remove();
        } else if (arg == QLatin1String(ALLOW_ROOT_OPTION)) {
            info.AllowRoot = true;
            it.remove();
        } else if (arg == QLatin1String(RESET_CONFIG_OPTION)) {
            info.ResetConfig = true;
            it.remove();
        } else if (arg == QLatin1String(PLUGIN_PATH_OPTION)) {
            it.remove();
            if (it.hasNext()) {
                qApp->addLibraryPath(it.next());
                it.remove();
            }
        }
    }
    if (hasHelp) {
        printHelp();
    }
    if (hasVersion) {
        printVersion();
    }

    // Prepare to start application
    info.checkLoadInfo();

    QSplashScreen splash(QPixmap(":/svsplash.png"));
    splash.show();

    // Prepare to load plugins
    QBreakpadInstance.setDumpPath(qsConf->appDataDir() + "/crashes");

    QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

    PlainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
