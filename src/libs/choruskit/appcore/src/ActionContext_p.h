#ifndef ACTIONCONTEXT_P_H
#define ACTIONCONTEXT_P_H

#include <QHash>

#include "ActionContext.h"
#include "QMChronMap.h"

namespace Core {

    class ActionSystem;

    class ActionContextItemPrivate {
    public:
        ActionContextPrivate *d;

        QString id;
        bool isGroup;
        QList<ActionInsertRule> rules;
        QSet<ActionItem *> instances;
    };

    class ActionContextPrivate {
        Q_DECLARE_PUBLIC(ActionContext)
    public:
        ActionContextPrivate();
        void init();

        ActionContext *q_ptr;

        QString id;
        QMDisplayString title;

        bool configurable;
        QMChronMap<QString, ActionContextItemPrivate> actions;

        mutable bool stateDirty;
        mutable QMap<QString, QStringList> state;

        void setDirty();
    };

}

#endif // ACTIONCONTEXT_P_H
