#ifndef ACTIONSYSTEM_P_H
#define ACTIONSYSTEM_P_H

#include <QHash>

#include "ActionSystem.h"

namespace Core {

    class ActionSystemPrivate {
        Q_DECLARE_PUBLIC(ActionSystem)
    public:
        ActionSystemPrivate();
        virtual ~ActionSystemPrivate();

        void init();

        ActionSystem *q_ptr;

        QMap<QString, ActionContext *> contexts;
    };
}

#endif // ACTIONSYSTEM_P_H
