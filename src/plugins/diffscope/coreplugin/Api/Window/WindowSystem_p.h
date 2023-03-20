#ifndef WINDOWSYSTEM_P_H
#define WINDOWSYSTEM_P_H

#include <QHash>
#include <QSet>

#include <list>

#include "WindowSystem.h"

namespace Core {

    class WindowSystemPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(WindowSystem)
    public:
        WindowSystemPrivate();
        virtual ~WindowSystemPrivate();

        void init();

        WindowSystem *q_ptr;

        QMap<QString, IWindowFactory *> windowFactories;

        std::list<IWindowAddOnFactory *> addOnFactories;
        QHash<IWindowAddOnFactory *, decltype(addOnFactories)::iterator> addOnIndexes;

        QSet<IWindow *> iWindows;

        void _q_iWindowClosed();
    };
}

#endif // WINDOWSYSTEM_P_H
