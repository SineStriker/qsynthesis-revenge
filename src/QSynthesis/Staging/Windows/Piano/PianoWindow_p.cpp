#include "PianoWindow_p.h"

PianoWindowPrivate::PianoWindowPrivate() {
}

PianoWindowPrivate::~PianoWindowPrivate() {
}

void PianoWindowPrivate::init() {
    Q_Q(PianoWindow);

    closeFlag = false;

    actions = new PianoActions(q);
    actions->setup();

    q->connect(actions, &PianoActions::triggered, q, &PianoWindow::_q_actionTriggered);
}

void PianoWindowPrivate::reloadStrings_helper() {
    actions->reloadStrings();
}
