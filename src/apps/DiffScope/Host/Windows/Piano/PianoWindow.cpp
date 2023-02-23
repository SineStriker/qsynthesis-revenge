#include "PianoWindow.h"
#include "PianoWindow_p.h"

PianoWindow::PianoWindow(QWidget *parent) : PianoWindow(*new PianoWindowPrivate(), parent) {
}

PianoWindow::~PianoWindow() {
}

PianoWindow::PianoWindow(PianoWindowPrivate &d, QWidget *parent) : ProjectWindow(d, parent) {
    d.init();
}

void PianoWindow::load(const QDspxModel &model) {
    qDebug() << model.metadata.version;
}

QDspxModel PianoWindow::currentProject() const {
    return {};
}

QDspxModel PianoWindow::currentTrack() const {
    return {};
}

QDspxModel PianoWindow::currentClip() const {
    return {};
}
