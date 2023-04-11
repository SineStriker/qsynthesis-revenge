#ifndef ACTIONSPEC_H
#define ACTIONSPEC_H

#include <QKeySequence>
#include <QObject>

#include "CkAppCoreGlobal.h"

namespace Core {

    class ActionSpecPrivate;

    class CKAPPCORE_API ActionSpec : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ActionSpec)
    public:
        explicit ActionSpec(const QString &id, QObject *parent = nullptr);
        ~ActionSpec();

    public:
        QString id() const;

        QString displayName() const;
        void setDisplayName(const QString &displayName);

        QString description() const;
        void setDescription(const QString &description);

        QList<QKeySequence> shortcuts() const;
        void setShortcuts(const QList<QKeySequence> &shortcuts);

    signals:
        void displayNameChanged(const QString &displayName);
        void descriptionChanged(const QString &description);
        void shortcutsChanged(const QList<QKeySequence> &shortcuts);

    protected:
        ActionSpec(ActionSpecPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ActionSpecPrivate> d_ptr;
    };

}

#endif // ACTIONSPEC_H