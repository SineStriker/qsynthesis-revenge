#include "BasicManager.h"
#include "BasicManager_p.h"

BasicManager::BasicManager(QObject *parent) : BasicManager(*new BasicManagerPrivate(), parent) {
}

BasicManager::~BasicManager() {
}

BasicManager::BasicManager(BasicManagerPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
