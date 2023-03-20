#ifndef IWINDOWADDON_P_H
#define IWINDOWADDON_P_H

#include "Global/CoreGlobal.h"

namespace Core {

    class IWindow;

    class CORE_EXPORT IWindowAddOnPrivate {
        Q_DECLARE_PUBLIC(IWindowAddOn)
    public:
        IWindowAddOnPrivate();
        virtual ~IWindowAddOnPrivate();

        void init();

        IWindowAddOn *q_ptr;

        IWindow *iWin;
    };
}

#endif // IWINDOWADDON_P_H
