#ifndef WINDOWMANAGERPRIVATE_H
#define WINDOWMANAGERPRIVATE_H

#include "../WindowManager.h"
#include "Basic/BasicManager_p.h"

#include "Windows/Basic/ProjectWindow.h"
#include "Windows/Home/HomeWindow.h"

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
    QSet<ProjectWindow *> projWins;
};

#endif // WINDOWMANAGERPRIVATE_H
