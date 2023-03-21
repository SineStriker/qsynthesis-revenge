#ifndef IPROJECTWINDOW_P_H
#define IPROJECTWINDOW_P_H

#include "ICoreWindow_p.h"
#include "IProjectWindow.h"

namespace Core {

    class IProjectWindowPrivate : public ICoreWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IProjectWindow)
    public:
        IProjectWindowPrivate();

        void init();
    };

}


#endif // IPROJECTWINDOW_P_H
