#include "HomeWindow_p.h"

#include "Kernel/QMDecorator.h"
#include "Kernel/DsLocalData.h"

#include "DsConsole.h"
#include "QMSystem.h"
#include "QMView.h"

#include <QLabel>

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
    deinit();
}

void HomeWindowPrivate::init() {
    Q_Q(HomeWindow);

    frame = new CNavFrame();
    frame->setObjectName("home-frame");
    q->setCentralWidget(frame);

    recentWidget = new HomeRecentWidget();
    recentWidget->setObjectName("home-recent-widget");

    recoveryWidget = new QLabel("Recovery");
    recoveryWidget->setObjectName("home-recovery-widget");

    recentButton = frame->addWidget(recentWidget);
    recentButton->setProperty("type", "home-nav-button");
    recentButton->setObjectName("home-recent-button");

    recoverButton = frame->addWidget(recoveryWidget);
    recoverButton->setProperty("type", "home-nav-button");
    recoverButton->setObjectName("home-recover-button");

    aboutButton = new CTabButton();
    aboutButton->setProperty("type", "home-bottom-button");
    aboutButton->setObjectName("home-about-button");
    frame->setBottomWidget(aboutButton);

    qIDec->installLocale(q, {"DsHost"}, _LOC(HomeWindow, q));
    qIDec->installTheme(q, {"Global", "HomeWindow"});

    // Init window size and width ratio
    double r = dsLocal->home.navWidthRatio;
    QRect rect(dsLocal->home.windowRect);

    if (rect.width() * rect.height() != 0) {
        // q->setGeometry(rect);
        QMView::centralizeWindow(q, {0.75, 0.75});
    } else {
        QMView::centralizeWindow(q, {0.75, 0.75});
        r = (225 * q->screen()->logicalDotsPerInch() / QMOs::unitDpi()) / q->width();
    }
    frame->splitter()->setSizes({int(q->width() * r), int(q->width() * (1 - r))});

    q->connect(aboutButton, &QPushButton::clicked, q, &HomeWindow::_q_aboutButtonClicked);
}

void HomeWindowPrivate::deinit() {
    Q_Q(HomeWindow);

    dsLocal->home.navWidthRatio = double(frame->splitter()->widget(0)->width()) / frame->width();
    dsLocal->home.windowRect = q->geometry();
}
