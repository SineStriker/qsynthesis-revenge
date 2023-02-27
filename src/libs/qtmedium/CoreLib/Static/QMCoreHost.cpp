#include "QMCoreHost.h"
#include "private/QMCoreHost_p.h"

Q_SINGLETON_DECLARE(QMCoreHost);

QMCoreHost::QMCoreHost(QObject *parent) : QMCoreHost(*new QMCoreHostPrivate(), parent) {
}

QMCoreHost::~QMCoreHost() {
}

bool QMCoreHost::isAboutToQuit() const {
    Q_D(const QMCoreHost);
    return d->isAboutToQuit;
}

QSettings::Format QMCoreHost::settingsJsonFormat() const {
    Q_D(const QMCoreHost);
    return d->settingsJsonFormat;
}

QMCoreHost::QMCoreHost(QMCoreHostPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}

void QMCoreHost::_q_appAboutToQuit() {
    Q_D(QMCoreHost);
    d->isAboutToQuit = true;
}