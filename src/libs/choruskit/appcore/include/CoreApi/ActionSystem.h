#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include <QMenuBar>

#include "ActionContext.h"

namespace Core {

    class ActionSpecPrivate;

    class CKAPPCORE_API ActionSpec {
    public:
        ActionSpec();

        bool isValid() const;
        inline operator bool() const {
            return isValid();
        }

        QString id() const;

        QString displayName() const;
        void setDisplayName(const QString &displayName);

        QString description() const;
        void setDescription(const QString &description);

        QList<QKeySequence> shortcuts() const;
        void setShortcuts(const QList<QKeySequence> &shortcuts);

    private:
        explicit ActionSpec(ActionSpecPrivate *d);
        ActionSpecPrivate *d;

        friend class ActionSystem;
    };

    class ActionSystemPrivate;

    class CKAPPCORE_API ActionSystem : public QObject {
        Q_OBJECT
    public:
        explicit ActionSystem(QObject *parent = nullptr);
        ~ActionSystem();

    public:
        ActionSpec addAction(const QString &id);
        void removeAction(const QString &id);
        ActionSpec action(const QString &id);
        QStringList actionIds() const;

        bool addContext(ActionContext *context);
        bool removeContext(ActionContext *context);
        bool removeContext(const QString &id);

        ActionContext *context(const QString &id) const;
        QList<ActionContext *> contexts() const;
        QStringList contextIds() const;

        QStringList loadContexts(const QString &filename);

    public:
        QMap<QString, QStringList> stateCache(const QString &id);
        void setStateCache(const QString &id, const QMap<QString, QStringList> &state);

    signals:
        void actionAdded(const QString &id);
        void actionRemoved(const QString &id);

        void actionDisplayNameChanged(const QString &id, const QString &displayName);
        void actionDescriptionChanged(const QString &id, const QString &description);
        void actionShortcutsChanged(const QString &id, const QList<QKeySequence> &shortcuts);

    protected:
        QScopedPointer<ActionSystemPrivate> d_ptr;
        ActionSystem(ActionSystemPrivate &d, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(ActionSystem)

        friend class ActionContext;
        friend class ActionItem;
    };

}

#endif // ACTIONSYSTEM_H
