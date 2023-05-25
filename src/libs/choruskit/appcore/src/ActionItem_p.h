#ifndef ACTIONITEM_P_H
#define ACTIONITEM_P_H

#include <QPointer>

#include "ActionItem.h"
#include "ActionSpec.h"

namespace Core {

    class ActionItemPrivate : public QObject {
        Q_DECLARE_PUBLIC(ActionItem)
    public:
        ActionItemPrivate();
        virtual ~ActionItemPrivate();

        void init();

        bool getSpec();

        ActionItem *q_ptr;

        ActionSpec *spec;

        QString id;
        ActionItem::Type type;

        bool autoDelete;

        QPointer<QAction> action;
        QPointer<QActionGroup> actionGroup;
        QPointer<QMenu> menu;
        QPointer<QWidgetAction> widget;

        QString commandCategory;
        QString commandName;
        QString commandDesc;

    private:
        void _q_actionShortcutsChanged(const QList<QKeySequence> &shortcuts);
    };

}

#endif // ACTIONITEM_P_H
