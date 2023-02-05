#include "HomeWindow_p.h"

#include "Kernel/CDecorator.h"
#include "Kernel/DsLocalData.h"

#include "QsSystem.h"
#include "QsView.h"

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
    qIDec->installTheme(q, {"HomeWindow"});

    // Init window size and width ratio
    double r = dsLocal->home.navWidthRatio;
    QRect rect(dsLocal->home.windowRect);
    if (rect.width() * rect.height() != 0) {
        q->setGeometry(rect);
    } else {
        QsView::centralizeWindow(q, {0.75, 0.75});
        r = (225 * q->screen()->logicalDotsPerInch() / QsOs::unitDpi()) / q->width();
    }
    frame->splitter()->setSizes({int(q->width() * r), int(q->width() * (1 - r))});
}

void HomeWindowPrivate::deinit() {
    Q_Q(HomeWindow);

    dsLocal->home.navWidthRatio = double(frame->splitter()->widget(0)->width()) / frame->width();
    dsLocal->home.windowRect = q->geometry();
}
