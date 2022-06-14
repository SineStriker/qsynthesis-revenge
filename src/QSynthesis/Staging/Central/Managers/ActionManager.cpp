#include "ActionManager.h"
#include "ActionManager_p.h"

#include "MainWindow.h"

ActionManager::ActionManager(MainWindow *parent)
    : ActionManager(*new ActionManagerPrivate(), parent) {
}

ActionManager::~ActionManager() {
}

bool ActionManager::load() {
    return true;
}

ActionManager::ActionManager(ActionManagerPrivate &d, QObject *parent) : BaseManager(d, parent) {
    d.init();
}
