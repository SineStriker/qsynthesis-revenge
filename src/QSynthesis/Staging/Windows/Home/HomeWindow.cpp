#include "HomeWindow.h"
#include "private/HomeWindow_p.h"

#include "Kernel/QsApplication.h"
#include "Managers/WindowManager.h"

#include "Managers/PluginManager.h"

#include <QDebug>

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
}

HomeWindow::~HomeWindow() {
}

void HomeWindow::reloadStrings() {
    Q_D(HomeWindow);
    d->reloadStrings_helper();
}

HomeWindow::HomeWindow(HomeWindowPrivate &d, QWidget *parent) : PlainWindow(d, parent) {
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
