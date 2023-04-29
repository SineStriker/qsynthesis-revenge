#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <QObject>

#include "IWindow.h"
#include "IWindowAddOn.h"

class QSplitter;

namespace Core {

    class WindowSystemPrivate;

    class CKAPPCORE_API WindowSystem : public QObject {
        Q_OBJECT
    public:
        explicit WindowSystem(QObject *parent = nullptr);
        ~WindowSystem();

    public:
        bool addWindow(IWindowFactory *factory);
        bool removeWindow(IWindowFactory *factory);
        bool removeWindow(const QString &id);
        QList<IWindowFactory *> windowFactories() const;
        void clearWindowFactories();

        bool addAddOn(IWindowAddOnFactory *factory);
        bool removeAddOn(IWindowAddOnFactory *factory);
        QList<IWindowAddOnFactory *> addOnFactories() const;
        void clearAddOnFactories();

        IWindow *createWindow(const QString &id, QWidget *parent = nullptr);
        IWindow *findWindow(QWidget *window) const;

        int count() const;
        QList<IWindow *> windows() const;
        IWindow *firstWindow() const;

    public:
        void loadWindowGeometry(const QString &id, QWidget *w, const QSize &fallback = {}) const;
        void saveWindowGeometry(const QString &id, QWidget *w);

        void loadSplitterSizes(const QString &id, QSplitter *s, const QList<int> &fallback = {}) const;
        void saveSplitterSizes(const QString &id, QSplitter *s);

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
