#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include <QDebug>

#include <QMDecoratorV2.h>

#include "ICore.h"

namespace Core {

    IProjectWindowPrivate::IProjectWindowPrivate() {
    }

    void IProjectWindowPrivate::init() {
    }

    void IProjectWindowPrivate::reloadStrings() {
    }

    void IProjectWindowPrivate::reloadMenuBar() {
        Q_Q(IProjectWindow);
        mainMenuCtx->buildMenuBarWithState(q->actionItems(), q->menuBar());
    }

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(*new IProjectWindowPrivate(), parent) {
    }

    IProjectWindow::~IProjectWindow() {
    }

    void IProjectWindow::setupWindow() {
        Q_D(IProjectWindow);

        auto win = window();
        win->setObjectName("project-window");
        win->setAcceptDrops(true);

        d->mainMenuCtx = ICore::instance()->actionSystem()->context("project.MainMenu");
    }

    void IProjectWindow::windowAddOnsFinished() {
        Q_D(IProjectWindow);

        auto win = window();

        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &IProjectWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();

        qIDec->installLocale(this, _LOC(IProjectWindowPrivate, d));
        qIDec->installTheme(win, "core.ProjectWindow");
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
