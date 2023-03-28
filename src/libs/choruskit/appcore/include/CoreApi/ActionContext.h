#ifndef ACTIONCONTEXT_H
#define ACTIONCONTEXT_H

#include <QMenuBar>
#include <QSet>

#include "CkAppCoreGlobal.h"

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

    class CKAPPCORE_API ActionContextData {
    public:
        bool isValid() const;

        QString id() const;
        bool isGroup() const;

        QList<ActionInsertRule> rules() const;
        void setRules(const QList<ActionInsertRule> &rules);

        QList<QKeySequence> shortcuts() const;
        void setShortcuts(const QList<QKeySequence> &shortcuts);

    private:
        explicit ActionContextData(ActionContextDataPrivate *d);
        ActionContextDataPrivate *d;

        friend class ActionContext;
        friend class ActionSystem;
    };

    class ActionContextPrivate;

    class CKAPPCORE_API ActionContext : public QObject {
        Q_OBJECT
    public:
        explicit ActionContext(const QString &id, QObject *parent = nullptr);
        ~ActionContext();

        QString id() const;
        virtual QString title() const;

        ActionContextData addAction(const QString &id, bool isGroup);
        void removeAction(const QString &id);
        ActionContextData action(const QString &id);

        bool configurable() const;
        void setConfigurable(bool configurable);

        QMap<QString, QStringList> state() const;

    signals:
        void actionAdded(const QString &id);
        void actionChanged(const QString &id);
        void actionRemoved(const QString &id);

    protected:
        ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionContextPrivate> d_ptr;

        Q_DECLARE_PRIVATE(ActionContext)

        friend class ActionSystem;
    };

}



#endif // ACTIONCONTEXT_H
