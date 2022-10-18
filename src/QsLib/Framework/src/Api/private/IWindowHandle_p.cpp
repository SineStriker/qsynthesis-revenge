#include "IWindowHandle_p.h"

IWindowHandlePrivate::IWindowHandlePrivate() {
}

IWindowHandlePrivate::~IWindowHandlePrivate() {
    w = nullptr;
}

void IWindowHandlePrivate::init() {
    Q_Q(IWindowHandle);

    w = qobject_cast<QMainWindow *>(q->parent());
}
