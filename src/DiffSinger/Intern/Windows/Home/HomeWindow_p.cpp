#include "HomeWindow_p.h"

#include "Kernel/CDecorator.h"

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
}

void HomeWindowPrivate::init() {
    Q_Q(HomeWindow);

    qIDec->installTheme(q, {"HomeWindow"});
}
