#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include <QMenuBar>

#include "ActionInsertRule.h"
#include "ActionItem.h"

namespace Core {

    namespace Internal {
        class ICorePrivate;
    }

    class ActionSystemPrivate;

    class CORE_EXPORT ActionSystem : public QObject {
        Q_OBJECT
    public:
        ActionInsertRuleSet rule(const QString &id);

        static void buildMenuBar(const QString &context, QList<ActionItem *> &actionItems, QMenuBar *menuBar);
        static void buildMenu(const QString &context, QList<ActionItem *> &actionItems, QMenu *menu);

    private:
        explicit ActionSystem(QObject *parent = nullptr);
        ~ActionSystem();

        QScopedPointer<ActionSystemPrivate> d_ptr;
        ActionSystem(ActionSystemPrivate &d, QObject *parent = nullptr);

        friend class ICore;
        friend class ICorePrivate;
    };

}

#endif // ACTIONSYSTEM_H
