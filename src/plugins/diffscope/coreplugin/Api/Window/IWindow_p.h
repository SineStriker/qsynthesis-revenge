#ifndef IWINDOW_P_H
#define IWINDOW_P_H

#include "IWindow.h"

namespace Core {

    class IWindowPrivate {
    public:
        IWindowPrivate();
        IWindowPrivate(IWindow::Type type);

        void init();

        IWindow *q_ptr;

        IWindow::Type type;

        QMap<QString, ActionItem *> actionItemMap;
    };
}



#endif // IWINDOW_P_H
