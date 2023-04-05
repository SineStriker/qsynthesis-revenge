#ifndef ACTIONSYSTEM_P_H
#define ACTIONSYSTEM_P_H

#include "ActionSystem.h"
#include "Collections/QMChronMap.h"

namespace Core {

    class ActionSystemPrivate {
        Q_DECLARE_PUBLIC(ActionSystem)
    public:
        ActionSystemPrivate();
        virtual ~ActionSystemPrivate();

        void init();

        ActionSystem *q_ptr;

        QMChronMap<QString, ActionContext *> contexts;
        QHash<QString, QMap<QString, QStringList>> stateCaches;
    };
}

#endif // ACTIONSYSTEM_P_H
