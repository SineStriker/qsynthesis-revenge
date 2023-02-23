#include "IWindowHandle_p.h"

QSAPI_USING_NAMESPACE

IWindowHandlePrivate::IWindowHandlePrivate() {
}

IWindowHandlePrivate::~IWindowHandlePrivate() {
    w = nullptr;
}

void IWindowHandlePrivate::init() {
    Q_Q(IWindowHandle);

    w = qobject_cast<QMainWindow *>(q->parent());

    titleBarFlags = IWindowHandle::TitleBarFlagMask;
}

void IWindowHandlePrivate::updateTitleBar() {
}
