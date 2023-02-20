#include "PlainWindow.h"
#include "private/PlainWindow_p.h"

QSAPI_USING_NAMESPACE

PlainWindow::PlainWindow(QWidget *parent) : PlainWindow(*new PlainWindowPrivate(), parent) {
}

PlainWindow::~PlainWindow() {
}

void PlainWindow::setMenuBar(QMenuBar *menuBar) {
    Q_D(PlainWindow);
    if (d->winHandle) {
        d->winHandle->setMenuBar(menuBar);
    } else {
        QMainWindow::setMenuBar(menuBar);
    }
}

QMenuBar *PlainWindow::menuBar() const {
    Q_D(const PlainWindow);
    if (d->winHandle) {
        return d->winHandle->menuBar();
    } else {
        return QMainWindow::menuBar();
    }
}

PlainWindow::PlainWindow(PlainWindowPrivate &d, QWidget *parent) : QMainWindow(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
