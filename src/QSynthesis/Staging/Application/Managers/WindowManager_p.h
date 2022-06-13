#ifndef WINDOWMANAGERPRIVATE_H
#define WINDOWMANAGERPRIVATE_H

#include "BaseManager_p.h"

#include "MainWindow.h"
#include "WindowManager.h"

class WindowManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(WindowManager)
public:
    WindowManagerPrivate();
    ~WindowManagerPrivate();

    void init();

    QSet<MainWindow *> windows;
};
#endif // WINDOWMANAGERPRIVATE_H
