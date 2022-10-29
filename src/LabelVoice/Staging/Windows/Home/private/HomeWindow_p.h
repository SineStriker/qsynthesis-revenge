#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Controls/Windows/PlainWindow_p.h"

#include "../HomeWindow.h"

#include "CTabButton.h"

#include <QWidget>
#include <QStackedWidget>

#include "../HomeMainWidget.h"
#include "../HomeProjConfWidget.h"

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    virtual ~HomeWindowPrivate();

    void init();

    void reloadStrings_helper();

    QStackedWidget *stack;

    HomeMainWidget *mainWidget;
    HomeProjConfWidget *projConfWidget;

    void cb_switchIn();
    void cb_switchOut();
};

#endif // HOMEWINDOWPRIVATE_H
