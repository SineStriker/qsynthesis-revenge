#include "QsAbstractManager.h"
#include "private/QsAbstractManager_p.h"

QsAbstractManager::QsAbstractManager(QObject *parent) : QsAbstractManager(*new QsAbstractManagerPrivate(), parent) {
}

QsAbstractManager::~QsAbstractManager() {
}

bool QsAbstractManager::load() {
    return true;
}

bool QsAbstractManager::save() {
    return true;
}

QsAbstractManager::QsAbstractManager(QsAbstractManagerPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
