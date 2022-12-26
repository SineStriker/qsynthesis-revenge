#include "HomeWindow.h"
#include "HomeWindow_p.h"

#include "Kernel/DsApplication.h"
#include "MultistyleHandle.h"

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
}

HomeWindow::~HomeWindow() {
}

void HomeWindow::reloadStrings(int locale) {
}

void HomeWindow::reloadScreen(int theme) {
    Q_D(HomeWindow);
    d->ld->removeThemes();
    switch (theme) {
        case MultistyleHandle::Light:
            d->ld->addTheme(":/themes/light/home-light.qss");
            break;
        case MultistyleHandle::Dark:
            d->ld->addTheme(":/themes/dark/home-dark.qss");
            break;
        default:
            break;
    }
    qApp->applyTheme(this, d->ld->stylesheetList);
}

HomeWindow::HomeWindow(HomeWindowPrivate &d, QWidget *parent) : PlainWindow(d, parent) {
    d.init();
}
