#ifndef ACTIONITEM_H
#define ACTIONITEM_H

#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QWidget>

#include "CkAppCoreGlobal.h"

namespace Core {

    class ActionItemPrivate;

    class CKAPPCORE_API ActionItem {
    public:
        enum Type {
            Invalid,
            Action,
            ActionGroup,
            Menu,
            Widget,
        };

        ActionItem(const QString &id, QAction *action);
        ActionItem(const QString &id, QActionGroup *actionGroup);
        ActionItem(const QString &id, QMenu *menu);
        ActionItem(const QString &id, QWidget *widget);
        ~ActionItem();

        QString id() const;
        Type type() const;

        QAction *action() const;
        QActionGroup *actionGroup() const;
        QMenu *menu() const;
        QWidget *widget() const;

        QIcon icon() const;
        void setIcon(const QIcon &icon) const;

        QString text() const;
        void setText(const QString &text) const;

        bool enabled() const;
        void setEnabled(bool enabled);

        bool autoDelete() const;
        void setAutoDelete(bool autoDelete);

    public:
        static bool autoDeleteGlobal();
        static void setAutoDeleteGlobal(bool autoDelete);

    protected:
        ActionItem(ActionItemPrivate &d, const QString &id);

        QScopedPointer<ActionItemPrivate> d_ptr;
    };

}

#endif // ACTIONITEM_H
