#include "WindowManager_p.h"

WindowManagerPrivate::WindowManagerPrivate() {
}

WindowManagerPrivate::~WindowManagerPrivate() {
}

void WindowManagerPrivate::init() {
    homeWin = nullptr;
}

HomeWindow *WindowManagerPrivate::createHomeWin() {
    Q_Q(WindowManager);
    auto w = new HomeWindow();
    w->installEventFilter(q);
    w->setAttribute(Qt::WA_DeleteOnClose);
    return w;
}

PianoWindow *WindowManagerPrivate::createPianoWin() {
    Q_Q(WindowManager);
    auto w = new PianoWindow();
    w->installEventFilter(q);
    w->setAttribute(Qt::WA_DeleteOnClose);
    return w;
}

