#include "QsStartInfo.h"

#include "QMConsole.h"
#include "QMSystem.h"
#include "QsCoreConfig.h"
#include "QsPluginManager.h"

#include "QMDecorateDir.h"

#include <QApplication>
#include <QDataStream>

Q_SINGLETON_DECLARE(QsStartInfo);

struct QsStartInfoPrivate {
    QsCoreConfig coreConfig;
    QsPluginManager *pluginMgr = nullptr;

    QMDecorateDir dd;
};

static const char Slash = '/';

static QString GetAppConfig() {
    return QMFs::binaryPath() + Slash + qApp->applicationName() + "_settings.json";
}

QsStartInfo::QsStartInfo(QObject *parent)
    : QObject(parent), AllowRoot(false), ResetConfig(false), d_ptr(new QsStartInfoPrivate()) {
    auto &d = *d_ptr;

    // Plugin manager
    d.pluginMgr = new QsPluginManager(this);

    // Load core config
    d.coreConfig.load(GetAppConfig());

    // Load qslib resources
    d.dd.setDir(d.coreConfig.shareDir() + Slash + "QsLib");
    d.dd.loadDefault("QsLib");
}

QsStartInfo::~QsStartInfo() {
}

bool QsStartInfo::checkLoadInfo() {
    auto &d = *d_ptr;

    qDebug() << "start_info: check user permissions";

    // Root privilege detection
    if (QMOs::isUserRoot() && !AllowRoot) {
        QString msg = QCoreApplication::tr("You're trying to start %1 as the %2, which is "
                                           "extremely dangerous and isn't recommended.")
                          .arg(qAppName(), QMOs::rootUserName());
        qmCon->MsgBox(nullptr, QMCoreConsole::Warning, qAppName(), msg);
        QMOs::exitApp(0);
    }

    // Reset configuration, exit
    if (ResetConfig) {
        d.coreConfig.saveDefault(GetAppConfig());
        QMOs::exitApp(0);
    }

    // Initialize app data and temp dirs
    if (!d.coreConfig.apply()) {
        ::exit(-1);
    }

    return true;
}
