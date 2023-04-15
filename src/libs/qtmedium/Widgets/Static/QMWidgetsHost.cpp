#include "QMWidgetsHost.h"
#include "private/QMWidgetsHost_p.h"

#include "private/QMWidgetsFactory.h"
#include "private/QMetaTypeImpl_p.h"

#include "QMDecoratorV2.h"

QMWidgetsHostPrivate::QMWidgetsHostPrivate() {
    Register_QMetaTypeImpl();
}

QMWidgetsHostPrivate::~QMWidgetsHostPrivate() {
}

void QMWidgetsHostPrivate::init() {
    // Add theme paths
    for (const auto &path : qAsConst(themePaths))
        qIDec->addThemePath(path);
}

QMCoreFactory *QMWidgetsHostPrivate::createFactory() {
    return new QMWidgetsFactory();
}

QMWidgetsHost::QMWidgetsHost(QObject *parent) : QMWidgetsHost(*new QMWidgetsHostPrivate(), parent) {
}

QMWidgetsHost::~QMWidgetsHost() {
}

QMWidgetsHost::QMWidgetsHost(QMWidgetsHostPrivate &d, QObject *parent) : QMCoreHost(d, parent) {
    d.init();
}