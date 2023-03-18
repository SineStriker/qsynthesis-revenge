#ifndef ACTIONCONTEXT_H
#define ACTIONCONTEXT_H

#include <QMenuBar>
#include <QSet>

#include "Global/CoreGlobal.h"

namespace Core {

    class ActionItem;

    class ActionInsertRule {
    public:
        enum InsertDirection {
            Append,
            Unshift,
        };

        QString id;
        InsertDirection direction;

        ActionInsertRule() : ActionInsertRule(QString()) {
        }

        ActionInsertRule(const QString &id) : ActionInsertRule(id, Append) {
        }

        ActionInsertRule(const QString &id, InsertDirection direction) : id(id), direction(direction) {
        }
    };

    class ActionContextDataPrivate;

    class CORE_EXPORT ActionContextData {
    public:
        bool isValid() const;

        QString id() const;
        QList<ActionInsertRule> &rules();
        QSet<ActionItem *> instances() const;

    private:
        explicit ActionContextData(ActionContextDataPrivate *d);
        ActionContextDataPrivate *d;

        friend class ActionContext;
        friend class ActionSystem;
    };

    class ActionContextPrivate;

    class CORE_EXPORT ActionContext : public QObject {
        Q_OBJECT
    public:
        explicit ActionContext(const QString &id, QObject *parent = nullptr);
        ~ActionContext();

        QString id() const;
        virtual QString title() const;

        ActionContextData addAction(const QString &id);
        void removeAction(const QString &id);
        ActionContextData action(const QString &id);

        void buildMenuBar(const QList<ActionItem *> &actionItems, QMenuBar *menuBar) const;
        void buildMenu(const QList<ActionItem *> &actionItems, QMenu *menu) const;

    protected:
        ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionContextPrivate> d_ptr;

        friend class ActionSystem;
    };

}



#endif // ACTIONCONTEXT_H
