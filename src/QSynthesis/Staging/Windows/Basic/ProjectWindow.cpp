#include "ProjectWindow.h"
#include "ProjectWindow_p.h"

#include <QApplication>
#include <QEvent>

#include "Kernel/Events.h"

ProjectWindow::ProjectWindow(QWidget *parent) : ProjectWindow(*new ProjectWindowPrivate(), parent) {
}

ProjectWindow::~ProjectWindow() {
}

ProjectWindow::ProjectWindow(ProjectWindowPrivate &d, QWidget *parent) : PlainWindow(d, parent) {
    d.init();
}

bool ProjectWindow::event(QEvent *event) {
    Q_D(ProjectWindow);
    switch (event->type()) {
        case QEvent::Hide: {
            QEventImpl::WindowCloseEvent e(d->closeFlag);
            QApplication::sendEvent(this, &e);
            break;
        }
        default:
            break;
    }
    return QMainWindow::event(event);
}

void ProjectWindow::closeEvent(QCloseEvent *event) {
}
