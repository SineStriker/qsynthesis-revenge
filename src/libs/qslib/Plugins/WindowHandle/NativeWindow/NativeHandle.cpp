#include "NativeHandle.h"
#include "private/NativeHandle_p.h"

#include <QEvent>

QSAPI_USING_NAMESPACE

NativeHandle::NativeHandle(QMainWindow *parent) : NativeHandle(*new NativeHandlePrivate(), parent) {
}

NativeHandle::~NativeHandle() {
}

void NativeHandle::setup() {
    Q_D(NativeHandle);
    d->setup_helper();
}

void NativeHandle::setMenuBar(QMenuBar *menuBar) {
    Q_D(NativeHandle);
    d->setMenuBar_helper(menuBar);
}

QMenuBar *NativeHandle::menuBar() const {
    Q_D(const NativeHandle);
    return d->titleBar->menuBar();
}

bool NativeHandle::eventFilter(QObject *obj, QEvent *event) {
    Q_D(NativeHandle);
    if (obj == d->w) {
        switch (event->type()) {
            case QEvent::WindowActivate:
                d->titleBar->setTitleBarActive(true);
                break;
            case QEvent::WindowDeactivate:
                d->titleBar->setTitleBarActive(false);
                break;
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

NativeHandle::NativeHandle(NativeHandlePrivate &d, QMainWindow *parent) : IWindowHandle(d, parent) {
    d.init();
}
