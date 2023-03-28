#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

namespace Core {

    IProjectWindowPrivate::IProjectWindowPrivate() {
    }

    void IProjectWindowPrivate::init() {
    }

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(*new IProjectWindowPrivate(), parent) {
    }

    IProjectWindow::~IProjectWindow() {
    }

    void IProjectWindow::setupWindow() {
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
