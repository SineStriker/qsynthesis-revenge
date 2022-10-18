#ifndef IWINDOWHANDLEPRIVATE_H
#define IWINDOWHANDLEPRIVATE_H

#include "../IWindowHandle.h"

class QSFRAMEWORK_API IWindowHandlePrivate {
    Q_DECLARE_PUBLIC(IWindowHandle)
public:
    IWindowHandlePrivate();
    virtual ~IWindowHandlePrivate();

    void init();

    IWindowHandle *q_ptr;

    QMainWindow *w;
};

#endif // IWINDOWHANDLEPRIVATE_H
