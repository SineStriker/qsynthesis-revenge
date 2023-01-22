#include "PianoWindow_p.h"

#include "CDecorator.h"

PianoWindowPrivate::PianoWindowPrivate() {
}

PianoWindowPrivate::~PianoWindowPrivate() {
}

void PianoWindowPrivate::init() {
    Q_Q(PianoWindow);

    ld = new LocalDecorator(q);
    ld->addTheme(QsDecorator::Dark, {":/themes/piano-dark.json"});
    ld->addTheme(QsDecorator::Light, {":/themes/piano-light.json"});
    ld->reloadScreen(qIDec->theme());

    Q_TR_NOTIFY_PRIVATE(PianoWindow);
}
