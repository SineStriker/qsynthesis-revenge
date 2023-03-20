#ifndef ICOREWINDOW_P_H
#define ICOREWINDOW_P_H

#include "ICoreWindow.h"
#include "IWindow_p.h"

namespace Core {

    class ICoreWindowPrivate : public IWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ICoreWindow)
    public:
        ICoreWindowPrivate();

        void init();
    };

}


#endif // ICOREWINDOW_P_H
