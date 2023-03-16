#ifndef ACTIONSYSTEM_P_H
#define ACTIONSYSTEM_P_H

#include <QHash>

#include "ActionSystem.h"

namespace Core {

    class ActionSystemPrivate {
    public:
        ActionSystemPrivate();

        void init();

        ActionSystem *q_ptr;

        QHash<QString, ActionInsertRuleSet> rules;
    };
}

#endif // ACTIONSYSTEM_P_H
