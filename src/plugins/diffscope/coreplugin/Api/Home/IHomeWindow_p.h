#ifndef IHOMEWINDOW_P_H
#define IHOMEWINDOW_P_H

#include "../Window/ICoreWindow_p.h"
#include "IHomeWindow.h"

#include "CNavFrame.h"

namespace Core {

    class IHomeWindowPrivate : public ICoreWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IHomeWindow)
    public:
        IHomeWindowPrivate();

        void init();

        CNavFrame *navFrame;
    };

}

#endif // IHOMEWINDOW_P_H
