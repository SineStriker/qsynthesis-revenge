#include "NativeHandle.h"
#include "private/NativeHandle_p.h"

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

NativeHandle::NativeHandle(NativeHandlePrivate &d, QMainWindow *parent) : IWindowHandle(d, parent) {
    d.init();
}
