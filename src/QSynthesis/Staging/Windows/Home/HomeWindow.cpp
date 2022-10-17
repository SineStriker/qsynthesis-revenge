#include "HomeWindow.h"
#include "HomeWindow_p.h"

#include "Application/Managers/DataManager.h"
#include "Application/Managers/WindowManager.h"

#include <QDebug>

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
}

HomeWindow::~HomeWindow() {
}

void HomeWindow::reloadStrings() {
    Q_D(HomeWindow);
    d->reloadStrings_helper();
}

HomeWindow::HomeWindow(HomeWindowPrivate &d, QWidget *parent) : BasicWindow(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();

    Q_TR_NOTIFY(HomeWindow)
}

void HomeWindow::_q_searchBoxChanged(const QString &text) {
    qDebug() << text;
}

void HomeWindow::_q_newButtonClicked() {
    qWindows->newProject();
}

void HomeWindow::_q_openButtonClicked() {
}
