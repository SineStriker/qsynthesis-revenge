#include "IHomeWindow.h"
#include "IHomeWindow_p.h"

#include <QCoreApplication>
#include <QStyle>

#include <QMDecoratorV2.h>
#include <QMBatch.h>
#include <QMView.h>

#include <CoreApi/ILoader.h>

#include "ICore.h"

namespace Core {

    static const char settingCategoryC[] = "Core/IHomeWindow";

    static IHomeWindow *m_instance = nullptr;

    IHomeWindowPrivate::IHomeWindowPrivate() {
        // navFrame = nullptr;
    }

    void IHomeWindowPrivate::init() {
    }

    IHomeWindowPrivate::~IHomeWindowPrivate() {
    }

    void IHomeWindowPrivate::reloadStrings() {
        Q_Q(IHomeWindow);
        q->setWindowTitle(tr("Welcome"));
        aboutButton->setText(tr("About %1").arg(qApp->applicationName()));
    }

    void IHomeWindowPrivate::_q_aboutButtonClicked() {
        Q_Q(IHomeWindow);
        ICore::aboutApp(q->window());
    }

    IHomeWindow *IHomeWindow::instance() {
        return m_instance;
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
        m_instance = nullptr;
    }

    void IHomeWindow::setupWindow() {
        Q_D(IHomeWindow);

        ICoreWindow::setupWindow();

        auto win = window();
        win->setObjectName("home-window");
        win->setAcceptDrops(true);

        auto frame = new CNavFrame();
        frame->setObjectName("home-frame");

        setCentralWidget(frame);
        d->cp->setParent(frame);

        auto titleButton = new CTabButton(qApp->applicationName());
        titleButton->setObjectName("home-title-button");
        titleButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        frame->setTopWidget(titleButton); //

        auto aboutButton = new CTabButton();
        aboutButton->setProperty("type", "home-bottom-button");
        aboutButton->setObjectName("home-about-button");
        frame->setBottomWidget(aboutButton);

        d->navFrame = frame;
        d->titleButton = titleButton;
        d->aboutButton = aboutButton;

        connect(aboutButton, &QAbstractButton::clicked, d, &IHomeWindowPrivate::_q_aboutButtonClicked);
    }

    void IHomeWindow::windowAddOnsFinished() {
        Q_D(IHomeWindow);

        ICoreWindow::windowAddOnsFinished();

        auto win = window();

        qIDec->installLocale(this, _LOC(IHomeWindowPrivate, d));
        qIDec->installTheme(win, "core.HomeWindow");

        // Load window sizes
        auto winMgr = ICore::instance()->windowSystem();
        winMgr->loadWindowGeometry(metaObject()->className(), win, {1200, 800});
        winMgr->loadSplitterSizes(metaObject()->className(), d->navFrame->splitter(), {250, win->width() - 250});

        // Set window maximized before all-ons initialized will cause wierd style polishing problems
        // ...
    }

    void IHomeWindow::windowAboutToClose() {
        Q_D(IHomeWindow);
        auto win = window();

        // Save window sizes
        auto winMgr = ICore::instance()->windowSystem();
        winMgr->saveSplitterSizes(metaObject()->className(), d->navFrame->splitter());
        winMgr->saveWindowGeometry(metaObject()->className(), win);
    }

    IHomeWindow::IHomeWindow(IHomeWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IHomeWindow::WindowTypeID(), parent) {
        m_instance = this;

        d.init();
    }

}
