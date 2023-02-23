#include "QMWidgetsHost_p.h"

#include "QMWidgetsFactory.h"
#include "private/QMetaTypeImpl_p.h"

#include <QCoreApplication>

QMWidgetsHostPrivate::QMWidgetsHostPrivate() {
    Register_QMetaTypeImpl();
}

QMWidgetsHostPrivate::~QMWidgetsHostPrivate() {
}

void QMWidgetsHostPrivate::init() {
}

QMCoreFactory *QMWidgetsHostPrivate::createFactory() {
    return new QMWidgetsFactory();
}