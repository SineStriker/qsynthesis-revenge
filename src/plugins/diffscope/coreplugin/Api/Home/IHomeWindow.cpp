#include "IHomeWindow.h"
#include "IHomeWindow_p.h"

#include <QDebug>
#include <QStyle>

#include "QMDecorator.h"

namespace Core {

    IHomeWindowPrivate::IHomeWindowPrivate() {
        navFrame = nullptr;
    }

    void IHomeWindowPrivate::init() {
    }

    void IHomeWindow::reloadStrings() {
    }

    QAbstractButton *IHomeWindow::addNavWidget(QWidget *w) {
        Q_D(IHomeWindow);
        return d->navFrame->addWidget(w);
    }

    IHomeWindow::IHomeWindow(QObject *parent) : IHomeWindow(*new IHomeWindowPrivate(), parent) {
    }

    IHomeWindow::~IHomeWindow() {
    }

    void IHomeWindow::setupWindow() {
        Q_D(IHomeWindow);

        auto win =  window();
        win->setObjectName("home-window");

        auto frame = new CNavFrame();
        frame->setObjectName("home-frame");
        setCentralWidget(frame);

        qIDec->installLocale(win, {{}}, _LOC(IHomeWindow, this));
        qIDec->installTheme(win, {"Global", "HomeWindow"});

        d->navFrame = frame;
    }

    IHomeWindow::IHomeWindow(IHomeWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IHomeWindow::WindowTypeID(), parent) {
        d.init();
    }

}
