#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Windows/private/PlainWindow_p.h"

#include "HomeWindow.h"

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    ~HomeWindowPrivate();

    void init();
};

#endif // HOMEWINDOWPRIVATE_H
