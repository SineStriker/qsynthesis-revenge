#include "NativeWindow.h"
#include "private/NativeWindow_p.h"

#include <FramelessWidgetsHelper>

FRAMELESSHELPER_USE_NAMESPACE

NativeWindow::NativeWindow(QWidget *parent, Qt::WindowFlags flags)
    : NativeWindow(*new NativeWindowPrivate(), parent, flags) {
}

NativeWindow::~NativeWindow() {
}

#ifndef Q_OS_MAC
void NativeWindow::setMenuBar(QMenuBar *menuBar) {
    Q_D(NativeWindow);
    auto helper = FramelessWidgetsHelper::get(this);
    auto orgBar = this->menuBar();
    if (orgBar) {
        helper->setHitTestVisible(orgBar, false);
    }
    helper->setHitTestVisible(menuBar);
    d->titleBar->setMenuBar(menuBar);
}

QMenuBar *NativeWindow::menuBar() const {
    Q_D(const NativeWindow);
    return d->titleBar->menuBar();
}
#endif

NativeWindow::NativeWindow(NativeWindowPrivate &d, QWidget *parent, Qt::WindowFlags flags)
    : Super(parent, flags), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
