#ifndef ACTIONCONTEXT_H
#define ACTIONCONTEXT_H

#include <QMenuBar>

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

    class ActionContextPrivate;

    class CORE_EXPORT ActionContext : public QObject {
        Q_OBJECT
    public:
        explicit ActionContext(const QString &id, QObject *parent = nullptr);
        ~ActionContext();

        QString id() const;
        virtual QString title() const;

        int rulesCount() const;
        QList<ActionInsertRule> rules() const;
        QList<ActionInsertRule> &rulesRef();
        void addRule(const ActionInsertRule &rule);

        void buildMenuBar(QList<ActionItem *> &actionItems, QMenuBar *menuBar) const;
        void buildMenu(QList<ActionItem *> &actionItems, QMenu *menu) const;

    protected:
        ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionContextPrivate> d_ptr;
    };

}



#endif // ACTIONCONTEXT_H
