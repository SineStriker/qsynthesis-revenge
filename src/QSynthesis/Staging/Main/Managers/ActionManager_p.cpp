#include "ActionManager_p.h"

#include "MainWindow.h"

#include "ActionManagerImpl/FilesActionEngine.h"
#include "ActionManagerImpl/FolderActionEngine.h"

#include "ActionManagerImpl/ActionManagerEngine_p.h"

ActionManagerPrivate::ActionManagerPrivate() {
}

ActionManagerPrivate::~ActionManagerPrivate() {
}

void ActionManagerPrivate::init() {
    Q_Q(ActionManager);

    if (w->editType() == MainWindow::Files) {
        engine = new FilesActionEngine(q);
    } else {
        engine = new FolderActionEngine(q);
    }

    // MenuBar
    engine->d_func()->menuBar = w->menuBar();

    // Managers
    engine->d_func()->eventMgr = w->eventMgr();
    engine->d_func()->tabMgr = w->tabMgr();
}
