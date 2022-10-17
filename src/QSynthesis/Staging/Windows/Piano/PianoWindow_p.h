#ifndef PIANOWINDOWPRIVATE_H
#define PIANOWINDOWPRIVATE_H

#include "PianoWindow.h"
#include "PianoActions.h"

class PianoWindowPrivate {
    Q_DECLARE_PUBLIC(PianoWindow)
public:
    PianoWindowPrivate();
    virtual ~PianoWindowPrivate();

    void init();

    void reloadStrings_helper();

    PianoWindow *q_ptr;

    bool closeFlag;

    PianoActions *actions;
};

#endif // PIANOWINDOWPRIVATE_H
