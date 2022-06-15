#ifndef ACTIONMANAGERENGINEPRIVATE_H
#define ACTIONMANAGERENGINEPRIVATE_H

#include "ActionManagerEngine.h"

#include <QMenuBar>

#include "../EventManager.h"
#include "../TabManager.h"

class ActionManagerEnginePrivate {
    Q_DECLARE_PUBLIC(ActionManagerEngine)
public:
    ActionManagerEnginePrivate();
    virtual ~ActionManagerEnginePrivate();

    void init();

    virtual void setup();
    virtual void reloadStrings();
    virtual void reloadShortcuts();

    QWidget *window() const;

    ActionManagerEngine *q_ptr;

    QMenuBar *menuBar;

    EventManager *eventMgr;
    TabManager *tabMgr;
};

#endif // ACTIONMANAGERENGINEPRIVATE_H
