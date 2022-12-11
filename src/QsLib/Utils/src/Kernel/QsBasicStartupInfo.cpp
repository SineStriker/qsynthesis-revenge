#include "QsBasicStartupInfo.h"

#include <QCoreApplication>
#include <QMetaType>

#include "QsNamespace.h"

Q_SINGLETON_DECLARE(QsBasicStartupInfo)

static bool hasInit = false;

static void registerQsNamespace() {
    qRegisterMetaType<Qs::Direction>("Qs::Direction");
    qRegisterMetaType<Qs::Priority>("Qs::Priority");
}

QsBasicStartupInfo::QsBasicStartupInfo() {
    construct();

    allowRootUser = false;
    allowSecondary = false;

    parser.addHelpOption();
    parser.addVersionOption();

    addInitializer(registerQsNamespace);
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

void QsBasicStartupInfo::addInitializer(const std::function<void()> &initializer) {
    initializers.append(initializer);
}

void QsBasicStartupInfo::initAll() {
    if (hasInit) {
        return;
    }
    hasInit = true;

    for (const auto &fun : qAsConst(initializers)) {
        fun();
    }
}
