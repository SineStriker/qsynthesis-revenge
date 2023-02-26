#include "QsStartInfo.h"

#include "QMConsole.h"
#include "QMSystem.h"
#include "QsCoreConfig.h"
#include "QsPluginManager.h"

#include "singleapplication.h"

#include "QMDecorateDir.h"

#include <QApplication>
#include <QDataStream>

Q_SINGLETON_DECLARE(QsStartInfo);

struct QsStartInfoPrivate {
    QsCoreConfig coreConfig;
    QsPluginManager *pluginMgr = nullptr;
    SingleApplication *hSingleApp = nullptr;

    QMDecorateDir dd;
};

static const char Slash = '/';

static const SingleApplication::Options opts =
    SingleApplication::ExcludeAppPath | SingleApplication::ExcludeAppVersion | SingleApplication::SecondaryNotification;

static const char ALLOW_ROOT_OPTION[] = "--allow-root";
static const char RESET_CONFIG_OPTION[] = "--allow-root";

static QString GetAppConfig() {
    return QMFs::binaryPath() + Slash + qApp->applicationName() + "_settings.json";
}

QsStartInfo::QsStartInfo(QObject *parent)
    : QObject(parent), AllowSecondary(false), AllowRoot(false), ResetConfig(false), d_ptr(new QsStartInfoPrivate()) {
    auto &d = *d_ptr;

    // Plugin manager
    d.pluginMgr = new QsPluginManager(this);

    // Load core config
    d.coreConfig.load(QMFs::binaryPath() + Slash + GetAppConfig());

    // Load qslib resources
    d.dd.setDir(d.coreConfig.appDir(QsCoreConfig::AppShare) + Slash + "QsLib");
    d.dd.loadDefault("QsLib");
}

QsStartInfo::~QsStartInfo() {
}

bool QsStartInfo::checkLoadInfo() {
    auto &d = *d_ptr;

    qDebug() << "start_info: check user permissions";

    // QStringList arguments = qApp->arguments(); // adapted arguments list is passed to plugin manager later
    // QMutableStringListIterator it(arguments);

    // bool allowRoot = false;
    // bool resetConfig = false;

    // while (it.hasNext()) {
    //     const QString &arg = it.next();
    //     if (arg == QLatin1String(ALLOW_ROOT_OPTION)) {
    //         it.remove();
    //         allowRoot = true;
    //     } else if (arg == QLatin1String(RESET_CONFIG_OPTION)) {
    //         it.remove();
    //         resetConfig = true;
    //     }
    // }

    // Root privilege detection
    if (QMOs::isUserRoot() && !AllowRoot) {
        QString msg = QCoreApplication::tr("You're trying to start %1 as the %2, which may cause "
                                           "security problem and isn't recommended.")
                          .arg(qAppName(), QMOs::rootUserName());
        qmCon->MsgBox(nullptr, QMCoreConsole::Warning, qAppName(), msg);
        QMOs::exitApp(0);
    }

    // Reset configuration, exit
    if (ResetConfig) {
        // coreConfig->saveDefault(configPath);
        QMOs::exitApp(0);
    }

    // Init Single Structures
    d.hSingleApp = new SingleApplication(qApp, true, opts);

    if (!AllowSecondary) {
        if (!d.hSingleApp->isPrimary()) {
            qInfo() << "start_info: primary instance already running. PID:" << d.hSingleApp->primaryPid();

            // This eventually needs moved into the NotepadNextApplication to keep
            // sending/receiving logic in the same place
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::WriteOnly);

            stream << qApp->arguments();
            d.hSingleApp->sendMessage(buffer);

            qInfo() << "start_info: secondary instance closing...";

            qApp->exit(0);

            ::exit(0);
        }

        qInfo() << "start_info: primary instance initializing...";
    }

    // Initialize app data and temp dirs
    if (!d.coreConfig.apply()) {
        ::exit(-1);
    }

    return true;
}
