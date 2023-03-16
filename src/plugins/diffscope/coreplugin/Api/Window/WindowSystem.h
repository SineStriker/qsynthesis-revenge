#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <QObject>

#include "IWindow.h"
#include "IWindowAddOn.h"

namespace Core {

    class WindowSystemPrivate;

    class CORE_EXPORT WindowSystem : public QObject {
        Q_OBJECT
    public:
        void addWindow(IWindowFactory *factory);
        void removeWindow(IWindowFactory *factory);
        void removeWindow(const QString &id);

        void addAddOn(IWindowAddOnFactory *factory);
        void removeAddOn(IWindowAddOnFactory *factory);

        IWindow *createWindow(const QString &id, QWidget *parent = nullptr);

        int count() const;
        QList<IWindow *> windows() const;

    signals:
        void windowCreated(IWindow *iWin);
        void windowDestroyed(IWindow *iWin);

    private:
        explicit WindowSystem(QObject *parent = nullptr);
        ~WindowSystem();

        QScopedPointer<WindowSystemPrivate> d_ptr;
        WindowSystem(WindowSystemPrivate &d, QObject *parent = nullptr);
    };
}



#endif // WINDOWSYSTEM_H
