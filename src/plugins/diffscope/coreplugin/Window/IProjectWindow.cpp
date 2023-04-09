#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include "ICore.h"

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

    void IProjectWindow::windowAddOnsFinished() {
        ICore::instance()->actionSystem()->context("project_MainMenu")->buildMenuBarWithState(actionItems(), menuBar());
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
