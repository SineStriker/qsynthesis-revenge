#include "CStartupInfo.h"

#include <QCoreApplication>
#include <QMetaType>

Q_SINGLETON_DECLARE(QsBasicStartupInfo)

QsBasicStartupInfo::QsBasicStartupInfo() {
    construct();

    allowRootUser = false;
    allowSecondary = false;

    parser.addHelpOption();
    parser.addVersionOption();
}

QsBasicStartupInfo::~QsBasicStartupInfo() {
}

QString QsBasicStartupInfo::mainTitle() const {
    return qApp->applicationName();
}

QString QsBasicStartupInfo::windowTitle() const {
    return mainTitle() + QString(" %1").arg(qApp->applicationVersion());
}

QString QsBasicStartupInfo::errorTitle() const {
    return QObject::tr("Error");
}
