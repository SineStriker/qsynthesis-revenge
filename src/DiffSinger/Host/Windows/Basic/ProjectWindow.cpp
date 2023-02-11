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

QString ProjectWindow::filename() const {
    Q_D(const ProjectWindow);
    return d->filename;
}

void ProjectWindow::setFilename(const QString &filename) {
    Q_D(ProjectWindow);
    d->filename = filename;
}

bool ProjectWindow::untitled() const {
    Q_D(const ProjectWindow);
    return d->untitled;
}

void ProjectWindow::setUntitled(bool untitled) {
    Q_D(ProjectWindow);
    d->untitled = untitled;
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
