#ifndef ACTIONCONTEXT_P_H
#define ACTIONCONTEXT_P_H

#include <QHash>

#include "ActionContext.h"
#include "QMChronSet.h"

namespace Core {

    class ActionSystem;

    class ActionContextDataPrivate {
    public:
        ActionContextPrivate *d;

        QString id;
        bool isGroup;
        QList<ActionInsertRule> rules;
        QList<QKeySequence> shortcuts;
        QSet<ActionItem *> instances;
    };

    class ActionContextPrivate {
        Q_DECLARE_PUBLIC(ActionContext)
    public:
        ActionContextPrivate();
        void init();

        ActionContext *q_ptr;

        QString id;
        bool isGroup;

        bool configurable;
        QMChronMap<QString, ActionContextDataPrivate> actions;

        mutable bool stateDirty;
        mutable QMap<QString, QStringList> state;
    };

}

#endif // ACTIONCONTEXT_P_H
