#include "HomeWindow_p.h"

#include "Kernel/CDecorator.h"

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
}

void HomeWindowPrivate::init() {
    Q_Q(HomeWindow);

    ld = new LocalDecorator(q);
    ld->addTheme(QsDecorator::Dark, {":/themes/home-dark.json"});
    ld->addTheme(QsDecorator::Light, {":/themes/home-light.json"});
    ld->reloadScreen(qIDec->theme());

    Q_TR_NOTIFY_PRIVATE(HomeWindow);
}
