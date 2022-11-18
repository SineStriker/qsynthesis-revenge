#include "MyStartupInfo.h"

#include <QCoreApplication>

Q_SINGLETON_DECLARE(MyStartupInfo)

MyStartupInfo::MyStartupInfo() {
    construct();

    allowRootUser = false;
    allowSecondary = false;

    parser.addHelpOption();
    parser.addVersionOption();
}

MyStartupInfo::~MyStartupInfo() {
}

QString MyStartupInfo::mainTitle() const {
    return qApp->applicationName();
}

QString MyStartupInfo::windowTitle() const {
    return mainTitle() + QString(" %1").arg(qApp->applicationVersion());
}

QString MyStartupInfo::errorTitle() const {
    return QObject::tr("Error");
}
