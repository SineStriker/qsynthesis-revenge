#ifndef ICOREWINDOW_P_H
#define ICOREWINDOW_P_H

#include "CoreApi/IWindow_p.h"

#include "ICoreWindow.h"

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
