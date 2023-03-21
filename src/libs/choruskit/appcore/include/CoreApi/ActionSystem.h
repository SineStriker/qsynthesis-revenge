#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include <QMenuBar>

#include "ActionContext.h"

namespace Core {

    class ActionSystemPrivate;

    class CKAPPCORE_API ActionSystem : public QObject {
        Q_OBJECT
    public:
        void addContext(ActionContext *context);
        void removeContext(ActionContext *context);
        void removeContext(const QString &id);

        ActionContext *context(const QString &id) const;
        QList<ActionContext *> contexts() const;
        QStringList contextIds() const;

    private:
        explicit ActionSystem(QObject *parent = nullptr);
        ~ActionSystem();

        QScopedPointer<ActionSystemPrivate> d_ptr;
        ActionSystem(ActionSystemPrivate &d, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(ActionSystem)

        friend class ICore;
        friend class ICorePrivate;
    };

}

#endif // ACTIONSYSTEM_H
