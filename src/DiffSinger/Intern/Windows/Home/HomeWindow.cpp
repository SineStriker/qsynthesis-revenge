#include "HomeWindow.h"
#include "HomeWindow_p.h"

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
}

HomeWindow::~HomeWindow() {
}

HomeWindow::HomeWindow(HomeWindowPrivate &d, QWidget *parent) : PlainWindow(d, parent) {
    d.init();
}
