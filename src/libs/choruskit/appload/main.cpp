#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QPushButton>

#include "QMWidgetsHost.h"
#include "QsCoreConfig.h"
#include "QsStartInfo.h"

#include "QBreakpadHandler.h"

#include "Windows/PlainWindow.h"

#include "cmdline.h"

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

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
    QStringList arguments = a.arguments(); // adapted arguments list is passed to plugin manager later
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
    if (hasHelp)
        printHelp();
    if (hasVersion)
        printVersion();

    // Prepare to start application
    info.checkLoadInfo();

    // Prepare to load plugins
    QBreakpadInstance.setDumpPath(qsConf->appDataDir() + "/crashes");

    QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

    PlainWindow w;
    w.show();

    return a.exec();
}
