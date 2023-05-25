
#ifndef ACTIONCONTEXT_H
#define ACTIONCONTEXT_H

#include <QMenuBar>
#include <QSet>
#include <QToolBar>

#include "CkAppCoreGlobal.h"

namespace Core {

    class ActionItem;

    class ActionInsertRule {
    public:
        enum InsertMode {
            Append,
            Unshift,
            InsertBehind,
            InsertFront,
        };

        QString id;
        InsertMode direction;

        ActionInsertRule() : ActionInsertRule(QString()) {
        }

        ActionInsertRule(const QString &id) : ActionInsertRule(id, Append) {
        }

        ActionInsertRule(const QString &id, InsertMode direction) : id(id), direction(direction) {
        }
    };

    class ActionContextItemPrivate;

    class CKAPPCORE_API ActionContextItem {
    public:
        ActionContextItem();

        bool isValid() const;
        inline operator bool() const {
            return isValid();
        }

        QString id() const;
        bool isGroup() const;

        QList<ActionInsertRule> rules() const;
        void setRules(const QList<ActionInsertRule> &rules);

    private:
        explicit ActionContextItem(ActionContextItemPrivate *d);
        ActionContextItemPrivate *d;

        friend class ActionContext;
        friend class ActionSystem;
    };

    class ActionContextPrivate;

    class CKAPPCORE_API ActionContext : public QObject {
        Q_OBJECT
    public:
        explicit ActionContext(const QString &id, QObject *parent = nullptr);
        ActionContext(const QString &id, const QString &title, QObject *parent = nullptr);
        ~ActionContext();

    public:
        QString id() const;

        QString title() const;
        void setTitle(const QString &title);

        ActionContextItem addAction(const QString &id, bool isGroup);
        void removeAction(const QString &id);
        ActionContextItem action(const QString &id);
        QStringList actionIds() const;

        bool configurable() const;
        void setConfigurable(bool configurable);

        QMap<QString, QStringList> state() const;

        void buildMenuBarWithState(const QList<ActionItem *> &items, QMenuBar *menuBar) const;
        void buildMenuWithState(const QList<ActionItem *> &items, QMenu *menu) const;
        void buildToolBarWithState(const QList<ActionItem *> &items, QToolBar *toolBar) const;

    signals:
        void titleChanged(const QString &title);
        void stateChanged();

    protected:
        ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionContextPrivate> d_ptr;

        Q_DECLARE_PRIVATE(ActionContext)

        friend class ActionSystem;
    };

}



#endif // ACTIONCONTEXT_H
