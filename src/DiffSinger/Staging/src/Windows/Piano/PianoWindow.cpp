#include "PianoWindow.h"
#include "PianoWindow_p.h"

#include "Kernel/DsApplication.h"
#include "MultistyleHandle.h"

PianoWindow::PianoWindow(QWidget *parent) : PianoWindow(*new PianoWindowPrivate(), parent) {
}

PianoWindow::~PianoWindow() {
}

PianoWindow::PianoWindow(PianoWindowPrivate &d, QWidget *parent) : ProjectWindow(d, parent) {
    d.init();
}

void PianoWindow::reloadStrings(int locale) {
}

void PianoWindow::reloadScreen(int theme) {
    Q_D(PianoWindow);
    d->ld->removeThemes();
    switch (theme) {
        case MultistyleHandle::Light:
            d->ld->addTheme(":/themes/light/piano-light.qss");
            break;
        case MultistyleHandle::Dark:
            d->ld->addTheme(":/themes/dark/piano-dark.qss");
            break;
        default:
            break;
    }
    qApp->applyTheme(this, d->ld->stylesheetList);
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
