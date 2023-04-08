#ifndef WINDOWSYSTEM_P_H
#define WINDOWSYSTEM_P_H

#include <QHash>
#include <QSet>

#include "Collections/QMChronMap.h"
#include "Collections/QMChronSet.h"

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

        QMChronMap<QString, IWindowFactory *> windowFactories;
        QMChronSet<IWindowAddOnFactory *> addOnFactories;

        QMChronSet<IWindow *> iWindows;
        QHash<QWidget *, IWindow *> windowMap;

    private:
        void _q_iWindowClosed();
    };
}

#endif // WINDOWSYSTEM_P_H
