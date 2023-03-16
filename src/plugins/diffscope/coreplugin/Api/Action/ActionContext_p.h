#ifndef ACTIONCONTEXT_P_H
#define ACTIONCONTEXT_P_H

#include "ActionContext.h"

namespace Core {

    class CORE_EXPORT ActionContextPrivate {
    public:
        ActionContextPrivate();
        void init();

        ActionContext *q_ptr;

        QString id;
        QList<ActionInsertRule> rules;
    };

}

#endif // ACTIONCONTEXT_P_H
