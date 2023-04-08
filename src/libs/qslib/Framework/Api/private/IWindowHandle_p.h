#ifndef IWINDOWHANDLEPRIVATE_H
#define IWINDOWHANDLEPRIVATE_H

#include "Api/IWindowHandle.h"

QSAPI_BEGIN_NAMESPACE

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

QSAPI_END_NAMESPACE

#endif // IWINDOWHANDLEPRIVATE_H
