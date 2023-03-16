#ifndef IWINDOW_H
#define IWINDOW_H

#include <QMainWindow>

#include "Action/ActionItem.h"
#include "Global/CoreGlobal.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class IWindow;
    class IWindowFactoryPrivate;

    class CORE_EXPORT IWindowFactory {
    public:
        enum AvailableCreator {
            Create,
            CreateWithId,
        };

        explicit IWindowFactory(const QString &id, AvailableCreator creator);
        ~IWindowFactory();

        QString id() const;
        AvailableCreator creator() const;

        virtual IWindow *create(QObject *parent = nullptr) = 0;
        virtual IWindow *create(const QString &id, QObject *parent = nullptr) = 0;

    private:
        QScopedPointer<IWindowFactoryPrivate> d_ptr;
    };

    class IWindowPrivate;

    class CORE_EXPORT IWindow : public QObject {
        Q_OBJECT
    public:
        explicit IWindow(const QString &id, QObject *parent = nullptr);
        ~IWindow();

        QString id() const;
        QMainWindow *window() const;

        void addActionItem(const QString &id, ActionItem *item);
        void removeActionItem(const QString &id);
        ActionItem *actionItem(const QString &id) const;
        QList<ActionItem *> actionItems() const;

        void reloadActions();

    signals:
        void aboutToShutdown();

    protected:
        virtual QMainWindow *createWindow(QWidget *parent) const;

    private:
        IWindow(IWindowPrivate &d, const QString &id, QObject *parent = nullptr);
        QScopedPointer<IWindowPrivate> d_ptr;

        friend class ICore;
        friend class ICorePrivate;
        friend class Internal::CorePlugin;
        friend class WindowSystem;
        friend class WindowSystemPrivate;
    };

}

#endif // IWINDOW_H
