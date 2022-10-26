#include "HomeWindow.h"
#include "private/HomeWindow_p.h"

#include "Kernel/LvApplication.h"
#include "Managers/WindowManager.h"

#include "Managers/PluginManager.h"

#include <QDebug>

#include "Serialization/QLVProject.h"

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
    LVModel::ProjectModel proj;

    qDebug() << proj.load("test.lvproj");
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
