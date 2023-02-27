#include "QsAbstractManager.h"
#include "private/QsAbstractManager_p.h"

QsAbstractManager::QsAbstractManager(QObject *parent) : QsAbstractManager(*new QsAbstractManagerPrivate(), parent) {
}

QsAbstractManager::~QsAbstractManager() {
}

QsAbstractManager::QsAbstractManager(QsAbstractManagerPrivate &d, QObject *parent) : QMDisposable(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
