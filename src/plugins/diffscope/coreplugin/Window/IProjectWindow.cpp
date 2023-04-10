#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include "ICore.h"

#include <QDebug>

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
        auto ctx = ICore::instance()->actionSystem()->context("project.MainMenu");
        if (!ctx) {
            qWarning() << "Core::IProjectWindow::windowAddOnsFinished(): menu context has been removed unexpectedly.";
        }
        ctx->buildMenuBarWithState(actionItems(), menuBar());
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
