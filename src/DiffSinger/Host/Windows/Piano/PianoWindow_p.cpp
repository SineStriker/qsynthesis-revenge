#include "PianoWindow_p.h"

#include "CDecorator.h"

PianoWindowPrivate::PianoWindowPrivate() {
}

PianoWindowPrivate::~PianoWindowPrivate() {
}

void PianoWindowPrivate::init() {
    Q_Q(PianoWindow);

    qIDec->installTheme(q, {"Global", "PianoWindow"});
}
