#ifndef PIANOWINDOWPRIVATE_H
#define PIANOWINDOWPRIVATE_H

#include "Windows/Basic/ProjectWindow_p.h"

#include "../PianoActions.h"
#include "../PianoWindow.h"

class PianoWindowPrivate : public ProjectWindowPrivate {
    Q_DECLARE_PUBLIC(PianoWindow)
public:
    PianoWindowPrivate();
    virtual ~PianoWindowPrivate();

    void init();

    void reloadStrings_helper();

    PianoActions *actions;
};

#endif // PIANOWINDOWPRIVATE_H
