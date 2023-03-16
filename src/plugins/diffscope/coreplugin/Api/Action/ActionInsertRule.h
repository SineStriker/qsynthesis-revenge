#ifndef ACTIONINSERTRULE_H
#define ACTIONINSERTRULE_H

#include <QMap>
#include <QString>

namespace Core {

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

    using ActionInsertRuleSet = QMap<QString, QList<ActionInsertRule>>; // key: context; value: rules;
}


#endif // ACTIONINSERTRULE_H
