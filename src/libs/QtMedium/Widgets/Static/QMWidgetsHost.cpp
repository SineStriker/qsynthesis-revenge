#include "QMWidgetsHost.h"
#include "private/QMWidgetsHost_p.h"

QMWidgetsHost::QMWidgetsHost(QObject *parent) : QMWidgetsHost(*new QMWidgetsHostPrivate(), parent) {
}

QMWidgetsHost::~QMWidgetsHost() {
}

QMWidgetsHost::QMWidgetsHost(QMWidgetsHostPrivate &d, QObject *parent) : QMCoreHost(d, parent) {
    d.init();
}