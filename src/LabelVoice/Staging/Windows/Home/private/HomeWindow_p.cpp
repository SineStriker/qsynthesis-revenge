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
    templateList->setObjectName("template-list");

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
    recentList->setObjectName("recent-list");

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
    subtitleLabel->setText(HomeWindow::tr("Select a template to create a project"));
    searchBox->setPlaceholderText(HomeWindow::tr("Search for recent projects"));

    q->setWindowTitle(HomeWindow::tr("Welcome to %1").arg(qApp->applicationName()));

    reloadTemplates();
}

void HomeWindowPrivate::reloadTemplates() {
    templateList->clear();

#define SET_AND_ADD(CFG, TITLE, SUBTITLE, ID)                                                      \
    CFG.title = TITLE;                                                                             \
    CFG.subtitle = SUBTITLE;                                                                       \
    CFG.id = ID;                                                                                   \
    templateList->addFileItem(CFG.icon, CFG.iconSize, CFG.id, CFG.title, CFG.subtitle, CFG.cont);

    SET_AND_ADD(emptyItemConfig, HomeWindow::tr("Empty Template"),
                HomeWindow::tr("Create empty project for marking"), 0);
    SET_AND_ADD(opencpopItemConfig, HomeWindow::tr("Opencpop Template"),
                HomeWindow::tr("Use Opencpop template for marking"), 1);
    SET_AND_ADD(diffItemConfig, HomeWindow::tr("DiffSinger Template"),
                HomeWindow::tr("Use DiffSinger template for marking"), 2);
    SET_AND_ADD(openvpiItemConfig, HomeWindow::tr("OpenVPI Template"),
                HomeWindow::tr("Use OpenVPI template for marking"), 3);

#undef SET_AND_ADD
}
