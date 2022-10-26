#ifndef IWINDOWHANDLEPRIVATE_H
#define IWINDOWHANDLEPRIVATE_H

#include "../IWindowHandle.h"

class QSFRAMEWORK_API IWindowHandlePrivate {
    Q_DECLARE_PUBLIC(IWindowHandle)
public:
    IWindowHandlePrivate();
    virtual ~IWindowHandlePrivate();

    void init();

    virtual void updateTitleBar();

    IWindowHandle *q_ptr;

    QMainWindow *w;

    IWindowHandle::TitleBarFlags titleBarFlags;
};

#endif // IWINDOWHANDLEPRIVATE_H
