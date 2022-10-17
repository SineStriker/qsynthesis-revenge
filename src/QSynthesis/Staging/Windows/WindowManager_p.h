#ifndef WINDOWMANAGERPRIVATE_H
#define WINDOWMANAGERPRIVATE_H

#include "Basic/BasicManager_p.h"
#include "WindowManager.h"

#include "Home/HomeWindow.h"
#include "Piano/PianoWindow.h"

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
