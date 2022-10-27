#include "PianoWindow_p.h"

PianoWindowPrivate::PianoWindowPrivate() {
}

PianoWindowPrivate::~PianoWindowPrivate() {
}

void PianoWindowPrivate::init() {
    Q_Q(PianoWindow);

    // Don't show icon
    if (winHandle) {
        winHandle->setTitleBarFlags(winHandle->titleBarFlags() & ~IWindowHandle::WindowIcon);
    }

    actions = new PianoActions(q);
    actions->setup();

    q->connect(actions, &PianoActions::triggered, q, &PianoWindow::_q_actionTriggered);
}

void PianoWindowPrivate::reloadStrings_helper() {
    actions->reloadStrings();
}
