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

    using ActionInsertRuleList = QList<ActionInsertRule>;

    class ActionContextPrivate;

    class CORE_EXPORT ActionContext : public QObject {
        Q_OBJECT
    public:
        explicit ActionContext(const QString &id, QObject *parent = nullptr);
        ~ActionContext();

        QString id() const;
        virtual QString title() const;

        void addRule(const QString &id, const ActionInsertRule &rule);
        void addRules(const QString &id, const ActionInsertRuleList &rules);
        void clearRules(const QString &id);
        ActionInsertRuleList rules(const QString &id) const;
        int rulesCount(const QString &id) const;

        void buildMenuBar(QList<ActionItem *> &actionItems, QMenuBar *menuBar) const;
        void buildMenu(QList<ActionItem *> &actionItems, QMenu *menu) const;

    protected:
        ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionContextPrivate> d_ptr;
    };

}



#endif // ACTIONCONTEXT_H
