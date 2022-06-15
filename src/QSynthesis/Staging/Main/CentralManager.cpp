#include "CentralManager.h"
#include "CentralManager_p.h"

#include "MainWindow.h"

CentralManager::CentralManager(MainWindow *parent)
    : CentralManager(*new CentralManagerPrivate(), parent) {
}

CentralManager::~CentralManager() {
}

CentralManager::CentralManager(CentralManagerPrivate &d, MainWindow *parent)
    : BaseManager(d, parent) {
    d.init();
}
