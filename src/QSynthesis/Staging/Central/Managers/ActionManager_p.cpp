#include "ActionManager_p.h"

#include "MainWindow.h"

ActionManagerPrivate::ActionManagerPrivate() {
}

ActionManagerPrivate::~ActionManagerPrivate() {
}

void ActionManagerPrivate::init() {
    Q_Q(ActionManager);

    w = qobject_cast<MainWindow *>(q->parent());
    Q_ASSERT(w);
}
