#ifndef PIANOWINDOWPRIVATE_H
#define PIANOWINDOWPRIVATE_H

#include "../Basic/ProjectWindow_p.h"

#include "PianoWindow.h"

class PianoWindowPrivate : public ProjectWindowPrivate {
    Q_DECLARE_PUBLIC(PianoWindow)
public:
    PianoWindowPrivate();
    ~PianoWindowPrivate();

    void init();
};

#endif // PIANOWINDOWPRIVATE_H
