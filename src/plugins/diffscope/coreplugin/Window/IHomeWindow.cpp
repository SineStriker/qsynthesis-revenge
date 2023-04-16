#include "IHomeWindow.h"
#include "IHomeWindow_p.h"

#include <QCoreApplication>
#include <QStyle>

#include <QMDecoratorV2.h>
#include <QMMath.h>
#include <QMView.h>

#include <CoreApi/ILoader.h>

#include "ICore.h"

namespace Core {

    static const char settingCategoryC[] = "Core/IHomeWindow";

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

        auto titleButton = new CTabButton(qAppName());
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

        qIDec->installLocale(this, _LOC(IHomeWindowPrivate, d));
        qIDec->installTheme(win, "core.HomeWindow");

        d->mainMenuCtx = ICore::instance()->actionSystem()->context("home.MainMenu");

        // Init window sizes
        {
            auto obj = ILoader::instance()->settings()->value(settingCategoryC).toObject();

            QRect winRect;
            winRect.setX(obj.value("x").toInt());
            winRect.setY(obj.value("y").toInt());
            winRect.setWidth(obj.value("width").toInt());
            winRect.setHeight(obj.value("height").toInt());

            double r = obj.value("sideRatio").toDouble();
            bool isMax = obj.value("isMaximized").toBool();

            if (winRect.size().isEmpty() || r == 0 || isMax) {
                // Adjust sizes
                win->resize(1200, 800);
                QMView::centralizeWindow(win);
                frame->splitter()->setSizes({250, 1000});
                if (isMax) {
                    win->showMaximized();
                }
            } else {
                win->setGeometry(winRect);
                frame->splitter()->setSizes({int(win->width() * r), int(win->width() * (1 - r))});
            }
        }
    }

    void IHomeWindow::windowAddOnsFinished() {
        Q_D(IHomeWindow);
        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &IHomeWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();
    }

    void IHomeWindow::windowAboutToClose() {
        Q_D(IHomeWindow);
        auto win = window();

        // Save window sizes
        {
            QJsonObject obj;
            obj.insert("x", win->x());
            obj.insert("y", win->y());
            obj.insert("width", win->width());
            obj.insert("height", win->height());
            obj.insert("sideRatio", double(d->navFrame->splitter()->widget(0)->width()) / win->width());
            obj.insert("isMaximized", win->isMaximized());
            ILoader::instance()->settings()->insert(settingCategoryC, obj);
        }
    }

    IHomeWindow::IHomeWindow(IHomeWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IHomeWindow::WindowTypeID(), parent) {
        d.init();
    }

}
