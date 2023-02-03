#include "HomeWindow_p.h"

#include "Kernel/CDecorator.h"

#include <QLabel>

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
}

void HomeWindowPrivate::init() {
    Q_Q(HomeWindow);

    frame = new CNavFrame();
    frame->setObjectName("home-frame");
    q->setCentralWidget(frame);

    recentWidget = new QLabel("Recent");
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
    aboutButton->setObjectName("home-about-button");
    frame->setBottomWidget(aboutButton);

    qIDec->installLocale(q, {"DsIntern"}, std::bind(&HomeWindow::reloadStrings, q));
    qIDec->installTheme(q, {"HomeWindow"});
}
