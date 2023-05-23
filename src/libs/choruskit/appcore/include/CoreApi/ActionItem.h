#ifndef ACTIONITEM_H
#define ACTIONITEM_H

#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QWidget>

#include "ActionSpec.h"

namespace Core {

    class ActionItemPrivate;

    class CKAPPCORE_API ActionItem : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ActionItem)
    public:
        enum Type {
            Invalid,
            Action,
            ActionGroup,
            Menu,
            Widget,
        };

        ActionItem(const QString &id, QAction *action, QObject *parent = nullptr);
        ActionItem(const QString &id, QActionGroup *actionGroup, QObject *parent = nullptr);
        ActionItem(const QString &id, QMenu *menu, QObject *parent = nullptr);
        ActionItem(const QString &id, QWidget *widget, QObject *parent = nullptr);
        ~ActionItem();

        QString id() const;
        Type type() const;
        ActionSpec *spec() const;

        inline bool isAction() const;
        inline bool isActionGroup() const;
        inline bool isMenu() const;
        inline bool isWidget() const;

        QAction *action() const;
        QActionGroup *actionGroup() const;
        QMenu *menu() const;
        QWidget *widget() const;

        QIcon icon() const;
        void setIcon(const QIcon &icon);

        QString text() const;
        void setText(const QString &text);

        bool enabled() const;
        void setEnabled(bool enabled);

        bool autoDelete() const;
        void setAutoDelete(bool autoDelete);

    public:
        QString commandDescription() const;
        void setCommandDescription(const QString &originalCommandName);

        QString commandName() const;
        void setCommandName(const QString &commandName);

    public:
        static bool autoDeleteGlobal();
        static void setAutoDeleteGlobal(bool autoDelete);

    protected:
        ActionItem(ActionItemPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionItemPrivate> d_ptr;
    };

    bool ActionItem::isAction() const {
        return type() == Action;
    }

    bool ActionItem::isActionGroup() const {
        return type() == ActionGroup;
    }

    bool ActionItem::isMenu() const {
        return type() == Menu;
    }

    bool ActionItem::isWidget() const {
        return type() == Widget;
    }
}

#endif // ACTIONITEM_H
