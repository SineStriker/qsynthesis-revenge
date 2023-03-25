#ifndef ACTIONCONTEXT_P_H
#define ACTIONCONTEXT_P_H

#include <QHash>

#include "ActionContext.h"
#include "QMChronSet.h"

namespace Core {

    class ActionSystem;

    class ActionContextDataPrivate {
    public:
        QString id;
        QList<ActionInsertRule> rules;
        QSet<ActionItem *> instances;
    };

    class ActionContextPrivate {
    public:
        ActionContextPrivate();
        void init();

        ActionContext *q_ptr;

        ActionSystem *system; // Maybe removed later...
        QString id;

        QMChronMap<QString, ActionContextDataPrivate> actions;
    };

}

#endif // ACTIONCONTEXT_P_H
