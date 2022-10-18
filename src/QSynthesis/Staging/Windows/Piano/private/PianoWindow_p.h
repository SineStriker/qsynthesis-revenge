#ifndef PIANOWINDOWPRIVATE_H
#define PIANOWINDOWPRIVATE_H

#include "Controls/Windows/PlainWindow_p.h"

#include "../PianoActions.h"
#include "../PianoWindow.h"

class PianoWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(PianoWindow)
public:
    PianoWindowPrivate();
    virtual ~PianoWindowPrivate();

    void init();

    void reloadStrings_helper();

    bool closeFlag;

    PianoActions *actions;
};

#endif // PIANOWINDOWPRIVATE_H
