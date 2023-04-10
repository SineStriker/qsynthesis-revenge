#ifndef ACTIONITEM_P_H
#define ACTIONITEM_P_H

#include <QPointer>

#include "ActionItem.h"

namespace Core {

    class ActionItemPrivate : public QObject {
        Q_DECLARE_PUBLIC(ActionItem)
    public:
        ActionItemPrivate();
        virtual ~ActionItemPrivate();

        void init();

        ActionItem *q_ptr;

        QString id;
        ActionItem::Type type;

        bool autoDelete;

        QPointer<QAction> action;
        QPointer<QActionGroup> actionGroup;
        QPointer<QMenu> menu;
        QPointer<QWidget> widget;

    private:
        void _q_actionDisplayNameChanged(const QString &id, const QString &displayName);
        void _q_actionDescriptionChanged(const QString &id, const QString &description);
        void _q_actionShortcutsChanged(const QString &id, const QList<QKeySequence> &shortcuts);
    };

}

#endif // ACTIONITEM_P_H
