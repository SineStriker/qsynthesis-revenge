#include "QMWidgetsHost_p.h"

#include "QMWidgetsFactory.h"
#include "private/QMetaTypeImpl_p.h"

#include "QMSystem.h"

#include <QCoreApplication>

QMWidgetsHostPrivate::QMWidgetsHostPrivate() {
    Register_QMetaTypeImpl();
}

QMWidgetsHostPrivate::~QMWidgetsHostPrivate() {
}

void QMWidgetsHostPrivate::init() {
    QStringList decs = confValues.value("Decorators", {});
    for (auto path : qAsConst(decs)) {
        if (QMFs::isPathRelative(path)) {
            path = confPrefix + "/" + path;
        }
        auto dec = QSharedPointer<QMDecorateDir>::create();
        if (dec->load(path)) {
            this->decs.append(dec);
        }
    }
}

QMCoreFactory *QMWidgetsHostPrivate::createFactory() {
    return new QMWidgetsFactory();
}