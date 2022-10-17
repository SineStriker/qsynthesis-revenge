#ifndef WINDOWMANAGERPRIVATE_H
#define WINDOWMANAGERPRIVATE_H

#include "Vendor/Objects/BasicManager_p.h"
#include "WindowManager.h"

#include "Windows/Home/HomeWindow.h"
#include "Windows/Piano/PianoWindow.h"

#include <QSet>

class WindowManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(WindowManager)
public:
    WindowManagerPrivate();
    virtual ~WindowManagerPrivate();

    void init();

    HomeWindow *createHomeWin();
    PianoWindow *createPianoWin();

    HomeWindow *homeWin;
    QSet<PianoWindow *> projWins;
};

#endif // WINDOWMANAGERPRIVATE_H
