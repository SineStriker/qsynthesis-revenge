#include "PianoWindow.h"
#include "PianoWindow_p.h"

PianoWindow::PianoWindow(QWidget *parent) : PianoWindow(*new PianoWindowPrivate(), parent) {
}

PianoWindow::~PianoWindow() {
}

PianoWindow::PianoWindow(PianoWindowPrivate &d, QWidget *parent) : ProjectWindow(d, parent) {
    d.init();
}

QString PianoWindow::filename() const {
    Q_D(const PianoWindow);
    return d->filename;
}

void PianoWindow::setFilename(const QString &filename) {
    Q_D(PianoWindow);
    d->filename = filename;
}

bool PianoWindow::load() {
    return false;
}

bool PianoWindow::save() {
    return false;
}
