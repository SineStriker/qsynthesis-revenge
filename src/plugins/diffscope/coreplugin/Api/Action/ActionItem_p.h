#ifndef ACTIONITEM_P_H
#define ACTIONITEM_P_H

#include "ActionItem.h"

namespace Core {

    class ActionItemPrivate {
    public:
        QString id;
        ActionItem::Type type;

        QAction *action;
        QActionGroup *actionGroup;
        QMenu *menu;
        QWidget *widget;

        ActionItem *q_ptr;

        ActionItemPrivate();

        void init();
    };

}

#endif // ACTIONITEM_P_H
