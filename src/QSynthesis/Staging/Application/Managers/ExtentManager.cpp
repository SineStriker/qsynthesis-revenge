#include "ExtentManager.h"
#include "ExtentManager_p.h"

Q_SINGLETON_DECLARE(ExtentManager)

ExtentManager::ExtentManager(QObject *parent) : ExtentManager(*new ExtentManagerPrivate(), parent) {
}

ExtentManager::~ExtentManager() {
}

bool ExtentManager::load() {
    return true;
}

bool ExtentManager::save() {
    return true;
}

void ExtentManager::reloadScreen() {
}

ExtentManager::ExtentManager(ExtentManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();

    d.init();
}
