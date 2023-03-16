#ifndef IWINDOW_H
#define IWINDOW_H

#include <QObject>

#include "Action/ActionItem.h"
#include "Global/CoreGlobal.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class IWindowPrivate;

    class CORE_EXPORT IWindow : public QObject {
        Q_OBJECT
    public:
        enum Type {
            Home,
            Project,
        };

        Type type() const;

        void addActionItem(const QString &id, ActionItem *item);
        void removeActionItem(const QString &id);
        ActionItem *actionItem(const QString &id) const;
        QList<ActionItem *> actionItems() const;

        void reloadActions();

    private:
        explicit IWindow(QObject *parent = nullptr);
        ~IWindow();

        IWindow(IWindowPrivate &d, QObject *parent = nullptr);
        QScopedPointer<IWindowPrivate> d_ptr;

        friend class ICore;
        friend class ICorePrivate;
        friend class Internal::CorePlugin;
    };

}

#endif // IWINDOW_H
