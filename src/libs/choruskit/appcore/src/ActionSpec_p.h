#ifndef ACTIONSPECPRIVATE_H
#define ACTIONSPECPRIVATE_H

#include "ActionSpec.h"

namespace Core {

    class ActionSpecPrivate {
        Q_DECLARE_PUBLIC(ActionSpec)
    public:
        ActionSpecPrivate();
        virtual ~ActionSpecPrivate();

        void init();

        ActionSpec *q_ptr;

        QString id;
        QString displayName;
        QString commandName;
        QList<QKeySequence> shortcuts;
    };

}

#endif // ACTIONSPECPRIVATE_H