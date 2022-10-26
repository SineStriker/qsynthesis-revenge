#include "IWindowHandle.h"
#include "private/IWindowHandle_p.h"

IWindowHandle::IWindowHandle(QMainWindow *parent)
    : IWindowHandle(*new IWindowHandlePrivate(), parent) {
}

IWindowHandle::~IWindowHandle() {
}

QMainWindow *IWindowHandle::window() const {
    Q_D(const IWindowHandle);
    return d->w;
}

void IWindowHandle::setup() {
}

void IWindowHandle::setMenuBar(QMenuBar *menuBar) {
    Q_D(IWindowHandle);
    d->w->setMenuBar(menuBar);
}

QMenuBar *IWindowHandle::menuBar() const {
    Q_D(const IWindowHandle);
    return d->w->menuBar();
}

void IWindowHandle::setTitleBarFlags(TitleBarFlags flags) {
    Q_D(IWindowHandle);
    d->titleBarFlags = flags;
    d->updateTitleBar();
}

IWindowHandle::TitleBarFlags IWindowHandle::titleBarFlags() {
    Q_D(const IWindowHandle);
    return d->titleBarFlags;
}

IWindowHandle::IWindowHandle(IWindowHandlePrivate &d, QMainWindow *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
