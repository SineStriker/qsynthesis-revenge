#include "ActionManager.h"
#include "ActionManager_p.h"

#include "MainWindow.h"

ActionManager::ActionManager(MainWindow *parent)
    : ActionManager(*new ActionManagerPrivate(), parent) {
}

ActionManager::~ActionManager() {
}

void ActionManager::reloadStrings() {
    Q_D(ActionManager);
    d->engine->reloadStrings();
}

bool ActionManager::load() {
    Q_D(ActionManager);

    d->engine->setup();

    return true;
}

ActionManager::ActionManager(ActionManagerPrivate &d, MainWindow *parent)
    : CentralManager(d, parent) {
    d.init();
}
