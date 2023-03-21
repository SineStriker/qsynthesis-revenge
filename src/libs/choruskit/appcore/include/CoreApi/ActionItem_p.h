#ifndef ACTIONITEM_P_H
#define ACTIONITEM_P_H

#include <QPointer>

#include "ActionItem.h"

namespace Core {

    class ActionItemPrivate {
    public:
        ActionItem *q_ptr;

        QString id;
        ActionItem::Type type;

        bool autoDelete;

        QPointer<QAction> action;
        QPointer<QActionGroup> actionGroup;
        QPointer<QMenu> menu;
        QPointer<QWidget> widget;

        ActionItemPrivate();
        virtual ~ActionItemPrivate();

        void init();
    };

}

#endif // ACTIONITEM_P_H
