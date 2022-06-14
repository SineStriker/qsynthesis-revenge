#include "BaseManager.h"
#include "BaseManager_p.h"

BaseManager::BaseManager(QObject *parent) : BaseManager(*new BaseManagerPrivate(), parent) {
}

BaseManager::~BaseManager() {
}

bool BaseManager::load() {
    return false;
}

bool BaseManager::save() {
    return false;
}

BaseManager::BaseManager(BaseManagerPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}
