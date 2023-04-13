#ifndef IHOMEWINDOW_P_H
#define IHOMEWINDOW_P_H

#include "ICoreWindow_p.h"
#include "IHomeWindow.h"

#include "CNavFrame.h"
#include "CTabButton.h"

#include <CoreApi/ActionContext.h>

namespace Core {

    class IHomeWindowPrivate : public ICoreWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IHomeWindow)
    public:
        IHomeWindowPrivate();
        ~IHomeWindowPrivate();

        void init();

        void reloadStrings();
        void reloadMenuBar();

        ActionContext *mainMenuCtx;

        CNavFrame *navFrame;
        CTabButton *aboutButton;

    private:
        void _q_aboutButtonClicked();
    };

}

#endif // IHOMEWINDOW_P_H
