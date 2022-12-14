#include "HomeWindow_p.h"

#include <QDir>

#include "Kernel/LvApplication.h"

#include "Managers/QsFileManager.h"

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
}

void HomeWindowPrivate::init() {
    Q_Q(HomeWindow);

    // Don't show icon and title
    if (winHandle) {
        winHandle->setTitleBarFlags(winHandle->titleBarFlags() & ~IWindowHandle::WindowIcon &
                                    ~IWindowHandle::WindowTitle);
    }

    mainWidget = new HomeMainWidget(q);
    mainWidget->setObjectName("home-main-widget");

    projConfWidget = new HomeProjConfWidget(q);
    projConfWidget->setObjectName("home-create-widget");

    stack = new QStackedWidget();
    stack->setObjectName("home-stack");

    stack->addWidget(mainWidget);
    stack->addWidget(projConfWidget);

    stack->setCurrentWidget(mainWidget);
    q->setCentralWidget(stack);

    q->connect(mainWidget->openButton, &QPushButton::clicked, q, &HomeWindow::_q_browseRequested);
    q->connect(mainWidget, &HomeMainWidget::newRequested, q, &HomeWindow::_q_newRequested);
    q->connect(mainWidget, &HomeMainWidget::openRequested, q, &HomeWindow::_q_openRequested);

    q->connect(projConfWidget, &HomeProjConfWidget::confirmed, q,
               &HomeWindow::_q_confirmProjectConfigure);
    q->connect(projConfWidget, &HomeProjConfWidget::canceled, q,
               &HomeWindow::_q_cancelProjectConfigure);
}

void HomeWindowPrivate::reloadStrings_helper() {
    Q_Q(HomeWindow);

    q->setWindowTitle(HomeWindow::tr("%1").arg(qApp->applicationName()));

    mainWidget->reloadStrings();
    projConfWidget->reloadStrings();

    mainWidget->reloadTemplates();
}

void HomeWindowPrivate::cb_switchIn() {
    projConfWidget->clear();
    stack->setCurrentWidget(projConfWidget);
}

void HomeWindowPrivate::cb_switchOut() {
    stack->setCurrentWidget(mainWidget);
}
