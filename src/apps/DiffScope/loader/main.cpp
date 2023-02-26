#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QPushButton>

#include "QMWidgetsHost.h"
#include "QsCoreConfig.h"
#include "QsStartInfo.h"

#include "QBreakpadHandler.h"

#include "Windows/PlainWindow.h"

#include "cmdline.h"

static const char appNameC[] = "DiffScope";
static const char corePluginNameC[] = "Core";

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName(appNameC);
    a.setApplicationVersion("0.0.0.0");
    a.setOrganizationName("ChorusKit");
    a.setOrganizationDomain("org.ChorusKit");

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
    QBreakpadHandler::UniqueExtraHandler = []() {
        qmCon->MsgBox(nullptr, QMConsole::Question, appNameC, "Unexpected exception happens, ready to crash!");
    };

    QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

    PlainWindow w;

    {
        auto btn = new QPushButton("Crash Now!");
        w.setCentralWidget(btn);
        QObject::connect(btn, &QPushButton::clicked, []() { qDebug() << reinterpret_cast<QString *>(1)->toInt(); });
    }

    // QMainWindow w;
    w.show();

    return a.exec();
}
