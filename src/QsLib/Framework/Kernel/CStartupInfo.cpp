#include "CStartupInfo.h"

#include <QCoreApplication>
#include <QMetaType>

Q_SINGLETON_DECLARE(CStartupInfo)

CStartupInfo::CStartupInfo() {
    construct();

    allowRootUser = false;
    allowSecondary = false;

    parser.addHelpOption();
    parser.addVersionOption();
}

CStartupInfo::~CStartupInfo() {
}

QString CStartupInfo::mainTitle() const {
    return qApp->applicationName();
}

QString CStartupInfo::windowTitle() const {
    return mainTitle() + QString(" %1").arg(qApp->applicationVersion());
}

QString CStartupInfo::errorTitle() const {
    return QObject::tr("Error");
}
