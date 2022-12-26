#include "PianoWindow_p.h"

PianoWindowPrivate::PianoWindowPrivate() {
}

PianoWindowPrivate::~PianoWindowPrivate() {
}

void PianoWindowPrivate::init() {
    Q_Q(PianoWindow);

    ld = new LocalDecorator(q);
}
