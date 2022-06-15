#ifndef ACTIONMANAGERPRIVATE_H
#define ACTIONMANAGERPRIVATE_H

#include "../CentralManager_p.h"

#include "ActionManager.h"
#include "ActionManagerImpl/ActionManagerEngine.h"

class ActionManagerPrivate : public CentralManagerPrivate {
    Q_DECLARE_PUBLIC(ActionManager)
public:
    ActionManagerPrivate();
    ~ActionManagerPrivate();

    void init();

    ActionManagerEngine *engine;
};

#endif // ACTIONMANAGERPRIVATE_H
