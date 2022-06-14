#ifndef ACTIONMANAGERPRIVATE_H
#define ACTIONMANAGERPRIVATE_H

#include "BaseManager_p.h"

#include "ActionManager.h"

class ActionManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(ActionManager)
public:
    ActionManagerPrivate();
    ~ActionManagerPrivate();

    void init();

    MainWindow *w;
};

#endif // ACTIONMANAGERPRIVATE_H
