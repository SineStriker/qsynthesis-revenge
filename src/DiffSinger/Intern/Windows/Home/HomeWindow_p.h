#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Tools/LocalDecorator.h"
#include "Windows/private/PlainWindow_p.h"

#include "HomeWindow.h"

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    ~HomeWindowPrivate();

    void init();

    LocalDecorator *ld;
};

#endif // HOMEWINDOWPRIVATE_H
