#ifndef WINDOWMANAGERPRIVATE_H
#define WINDOWMANAGERPRIVATE_H

#include "../WindowManager.h"
#include "Managers/private/QsAbstractManager_p.h"

#include "Windows/Basic/ProjectWindow.h"
#include "Windows/Home/HomeWindow.h"

#include <set>

class WindowManagerPrivate : public QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(WindowManager)
public:
    WindowManagerPrivate();
    virtual ~WindowManagerPrivate();

    void init();

    HomeWindow *createHomeWin();
    PianoWindow *createPianoWin();

    HomeWindow *homeWin;
    std::set<ProjectWindow *> projWins;
};

#endif // WINDOWMANAGERPRIVATE_H
