#include "ProjectWindow.h"
#include "ProjectWindow_p.h"

#include <QApplication>
#include <QCloseEvent>
#include <QEvent>

#include "Kernel/Events.h"

ProjectWindow::ProjectWindow(QWidget *parent) : ProjectWindow(*new ProjectWindowPrivate(), parent) {
}

ProjectWindow::~ProjectWindow() {
}

ProjectWindow::ProjectWindow(ProjectWindowPrivate &d, QWidget *parent) : PlainWindow(d, parent) {
    d.init();
}

void ProjectWindow::closeEvent(QCloseEvent *event) {
    Q_D(ProjectWindow);
    if (event->isAccepted()) {
        QEventImpl::WindowCloseEvent e(d->closeFlag);
        QApplication::sendEvent(this, &e);
    }
}
