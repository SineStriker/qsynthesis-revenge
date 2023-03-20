#ifndef ACTIONITEM_P_H
#define ACTIONITEM_P_H

#include "ActionItem.h"

namespace Core {

    class ActionItemPrivate {
    public:
        ActionItem *q_ptr;

        QString id;
        ActionItem::Type type;

        QAction *action;
        QActionGroup *actionGroup;
        QMenu *menu;
        QWidget *widget;

        ActionItemPrivate();
        virtual ~ActionItemPrivate();

        void init();
    };

}

#endif // ACTIONITEM_P_H
