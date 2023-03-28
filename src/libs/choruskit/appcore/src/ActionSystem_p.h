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

        QHash<QString, ActionContext *> contexts;
        QHash<QString, QMap<QString, QStringList>> stateCaches;
    };
}

#endif // ACTIONSYSTEM_P_H
