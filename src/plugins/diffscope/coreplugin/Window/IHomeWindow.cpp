#include "IHomeWindow.h"
#include "IHomeWindow_p.h"

#include <QCoreApplication>
#include <QStyle>

#include "ICore.h"
#include "QMDecorator.h"

namespace Core {

    IHomeWindowPrivate::IHomeWindowPrivate() {
        // navFrame = nullptr;
    }

    void IHomeWindowPrivate::init() {
    }

    void IHomeWindowPrivate::reloadStrings() {
        Q_Q(IHomeWindow);
        auto win = q->window();

        q->setWindowTitle(tr("Welcome"));
        aboutButton->setText(tr("About %1").arg(qAppName()));
    }

    void IHomeWindowPrivate::reloadMenuBar() {
        Q_Q(IHomeWindow);
        mainMenuCtx->buildMenuBarWithState(q->actionItems(), q->menuBar());
    }

    void IHomeWindowPrivate::_q_aboutButtonClicked() {
        Q_Q(IHomeWindow);
        ICore::aboutApp(q->window());
    }

    QAbstractButton *IHomeWindow::addNavWidget(QWidget *w) {
        Q_D(IHomeWindow);
        auto btn = d->navFrame->addWidget(w);
        btn->setProperty("type", "home-nav-button");
        return btn;
    }

    IHomeWindow::IHomeWindow(QObject *parent) : IHomeWindow(*new IHomeWindowPrivate(), parent) {
    }

    IHomeWindow::~IHomeWindow() {
    }

    void IHomeWindow::setupWindow() {
        Q_D(IHomeWindow);

        auto win = window();
        win->setObjectName("home-window");

        auto frame = new CNavFrame();
        frame->setObjectName("home-frame");
        setCentralWidget(frame);

        auto aboutButton = new CTabButton();
        aboutButton->setProperty("type", "home-bottom-button");
        aboutButton->setObjectName("home-about-button");
        frame->setBottomWidget(aboutButton);

        d->navFrame = frame;
        d->aboutButton = aboutButton;

        connect(aboutButton, &QAbstractButton::clicked, d, &IHomeWindowPrivate::_q_aboutButtonClicked);

        qIDec->installLocale(this, {{}}, _LOC(IHomeWindowPrivate, d));
        qIDec->installTheme(win, {"Global", "HomeWindow"});

        d->mainMenuCtx = ICore::instance()->actionSystem()->context("home.MainMenu");
    }

    void IHomeWindow::windowAddOnsFinished() {
        Q_D(IHomeWindow);
        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &IHomeWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();
    }

    IHomeWindow::IHomeWindow(IHomeWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IHomeWindow::WindowTypeID(), parent) {
        d.init();
    }

}
