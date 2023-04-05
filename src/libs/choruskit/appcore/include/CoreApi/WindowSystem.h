#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <QObject>

#include "IWindow.h"
#include "IWindowAddOn.h"

namespace Core {

    class WindowSystemPrivate;

    class CKAPPCORE_API WindowSystem : public QObject {
        Q_OBJECT
    public:
        explicit WindowSystem(QObject *parent = nullptr);
        ~WindowSystem();

    public:
        void addWindow(IWindowFactory *factory);
        void removeWindow(IWindowFactory *factory);
        void removeWindow(const QString &id);
        QList<IWindowFactory *> windowFactories() const;
        void removeWindowFactories();

        void addAddOn(IWindowAddOnFactory *factory);
        void removeAddOn(IWindowAddOnFactory *factory);
        QList<IWindowAddOnFactory *> addOnFactories() const;
        void removeAddOnFactories();

        IWindow *createWindow(const QString &id, QWidget *parent = nullptr);

        int count() const;
        QList<IWindow *> windows() const;
        IWindow *firstWindow() const;

    signals:
        void windowCreated(IWindow *iWin);
        void windowDestroyed(IWindow *iWin);

    protected:
        QScopedPointer<WindowSystemPrivate> d_ptr;
        WindowSystem(WindowSystemPrivate &d, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(WindowSystem)

        friend class IWindow;
        friend class IWindowPrivate;
    };
}



#endif // WINDOWSYSTEM_H