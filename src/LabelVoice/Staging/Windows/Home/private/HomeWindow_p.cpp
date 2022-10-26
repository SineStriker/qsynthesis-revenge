#include "HomeWindow_p.h"

#include <QCoreApplication>

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

    w = new QFrame();
    w->setObjectName("home-widget");
    q->setCentralWidget(w);

    // Left
    titleLabel = new CTabButton();
    titleLabel->setObjectName("title-label");

    subtitleLabel = new QLabel();
    subtitleLabel->setObjectName("subtitle-label");
    subtitleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    templateList = new FileListWidget();

    leftLayout = new QVBoxLayout();
    leftLayout->setMargin(0);
    leftLayout->setSpacing(0);

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(subtitleLabel);
    leftLayout->addWidget(templateList);

    leftWidget = new QFrame();
    leftWidget->setObjectName("home-welcome-widget");
    leftWidget->setLayout(leftLayout);

    // Right
    searchBox = new QLineEdit();
    searchBox->setObjectName("search-box");

    recentList = new FileListWidget();

    rightLayout = new QVBoxLayout();
    rightLayout->setMargin(0);
    rightLayout->setSpacing(0);

    rightLayout->addWidget(searchBox);
    rightLayout->addWidget(recentList);

    rightWidget = new QFrame();
    rightWidget->setObjectName("home-recent-widget");
    rightWidget->setLayout(rightLayout);

    // Main
    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("home-splitter");
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);

    mainLayout->addWidget(splitter);

    w->setLayout(mainLayout);

    q->connect(searchBox, &QLineEdit::textChanged, q, &HomeWindow::_q_searchBoxChanged);
}

void HomeWindowPrivate::reloadStrings_helper() {
    Q_Q(HomeWindow);

    titleLabel->setText(qApp->applicationName());
    subtitleLabel->setText(HomeWindow::tr("Select a template"));
    searchBox->setPlaceholderText(HomeWindow::tr("Search recent files"));

    q->setWindowTitle(HomeWindow::tr("Welcome to %1").arg(qApp->applicationName()));
}
