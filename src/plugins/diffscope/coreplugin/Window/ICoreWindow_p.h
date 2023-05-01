#ifndef ICOREWINDOW_P_H
#define ICOREWINDOW_P_H

#include <QSet>

#include "ICoreWindow.h"

#include <CoreApi/ActionContext.h>
#include <CoreApi/IWindow_p.h>

namespace Core {

    class ICoreWindowPrivate : public IWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ICoreWindow)
    public:
        ICoreWindowPrivate();

        void init();

        void reloadMenuBar();

        ActionContext *mainMenuCtx;
        QSet<QKeySequence> shortcutMap;
    };

}


#endif // ICOREWINDOW_P_H
