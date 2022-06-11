#include "BaseManager.h"

BaseManager::BaseManager(QObject *parent) : QObject(parent) {
}

BaseManager::~BaseManager() {
}

bool BaseManager::load() {
    return false;
}

bool BaseManager::save() {
    return false;
}
