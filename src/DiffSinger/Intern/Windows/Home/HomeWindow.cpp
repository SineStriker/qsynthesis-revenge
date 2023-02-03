#include "HomeWindow.h"
#include "HomeWindow_p.h"

#include <QApplication>

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
}

HomeWindow::~HomeWindow() {
}

void HomeWindow::reloadStrings() {
    Q_D(HomeWindow);
    d->recentButton->setText(tr("Recent"));
    d->recoverButton->setText(tr("Recovery"));
    d->aboutButton->setText(tr("About %1").arg(qAppName()));
}

HomeWindow::HomeWindow(HomeWindowPrivate &d, QWidget *parent) : PlainWindow(d, parent) {
    d.init();
}
