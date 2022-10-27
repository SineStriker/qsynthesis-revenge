#include "HomeWindow_p.h"

#include <QDir>

#include "Kernel/LvApplication.h"

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
}

void HomeWindowPrivate::init() {
    // Don't show icon and title
    if (winHandle) {
        winHandle->setTitleBarFlags(winHandle->titleBarFlags() & ~IWindowHandle::WindowIcon &
                                    ~IWindowHandle::WindowTitle);
    }

    memset(&cb, 0, sizeof(cb));
    w = nullptr;

    initHome();
}

void HomeWindowPrivate::initHome() {
    Q_Q(HomeWindow);

    w = new QFrame();
    w->setObjectName("home-widget");
    q->setCentralWidget(w);

    // Left
    titleLabel = new CTabButton();
    titleLabel->setObjectName("title-label");

    subtitleLabel = new QLabel();
    subtitleLabel->setObjectName("subtitle-label");
    subtitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    openButton = new CPushButton();
    openButton->setObjectName("open-button");

    subtitleButtonLayout = new QHBoxLayout();
    subtitleButtonLayout->setMargin(0);
    subtitleButtonLayout->setSpacing(0);

    subtitleButtonLayout->addWidget(subtitleLabel);
    subtitleButtonLayout->addWidget(openButton);

    templateList = new FileListWidget();
    templateList->setObjectName("template-list");

    leftLayout = new QVBoxLayout();
    leftLayout->setMargin(0);
    leftLayout->setSpacing(0);

    leftLayout->addWidget(titleLabel);
    leftLayout->addLayout(subtitleButtonLayout);
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

    q->connect(openButton, &QPushButton::clicked, q, &HomeWindow::_q_openButtonClicked);
    q->connect(searchBox, &QLineEdit::textChanged, q, &HomeWindow::_q_searchBoxChanged);
    q->connect(templateList->delegate(), &FileListItemDelegate::clicked, q,
               &HomeWindow::_q_templateItemClicked);
}

void HomeWindowPrivate::reloadStrings_helper() {
    Q_Q(HomeWindow);

    titleLabel->setText(qApp->applicationName());
    subtitleLabel->setText(HomeWindow::tr("Select a template to create a project"));
    searchBox->setPlaceholderText(HomeWindow::tr("Search for recent projects"));

    q->setWindowTitle(HomeWindow::tr("Welcome to %1").arg(qApp->applicationName()));

    reloadTemplates();

    if (cb.w) {
        cb_reloadStrings();
    }
}

void HomeWindowPrivate::reloadTemplates() {
    templateList->clear();

    openButton->setText(HomeWindow::tr("Open Project"));

#define SET_AND_ADD(CFG, TITLE, SUBTITLE, ID)                                                      \
    CFG.title = TITLE;                                                                             \
    CFG.subtitle = SUBTITLE;                                                                       \
    CFG.id = ID;                                                                                   \
    templateList->addFileItem(CFG.icon, CFG.iconSize, CFG.id, CFG.title, CFG.subtitle, CFG.cont);

    SET_AND_ADD(emptyItemConfig, HomeWindow::tr("Empty Template"),
                HomeWindow::tr("Create empty project for marking"), Empty);
    SET_AND_ADD(opencpopItemConfig, HomeWindow::tr("Opencpop Template"),
                HomeWindow::tr("Use Opencpop template for marking"), Opencpop);
    SET_AND_ADD(diffItemConfig, HomeWindow::tr("DiffSinger Template"),
                HomeWindow::tr("Use DiffSinger template for marking"), DiffSinger);
    SET_AND_ADD(openvpiItemConfig, HomeWindow::tr("OpenVPI Template"),
                HomeWindow::tr("Use OpenVPI template for marking"), OpenVPI);

#undef SET_AND_ADD
}

void HomeWindowPrivate::cb_init() {
    Q_Q(HomeWindow);

    cb.w = new QFrame();
    cb.w->setObjectName("home-create-widget");

    cb.titleLabel = new QLabel();
    cb.titleLabel->setObjectName("title-label");

    cb.subtitleLabel = new QLabel();
    cb.subtitleLabel->setObjectName("subtitle-label");
    cb.subtitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    cb.projectName = new LabelTextBlock(false);
    cb.projectName->setObjectName("project-name-block");

    cb.location = new LabelTextBlock(true);
    cb.projectName->setObjectName("location-block");

    cb.solution = new LabelTextBlock(false);
    cb.solution->setObjectName("solution-block");

    cb.backButton = new CPushButton();
    cb.backButton->setObjectName("prev-button");

    cb.createButton = new CPushButton();
    cb.createButton->setObjectName("ok-button");

    cb.buttonLayout = new QHBoxLayout();
    cb.buttonLayout->setMargin(0);
    cb.buttonLayout->setSpacing(0);

    cb.buttonLayout->addStretch();
    cb.buttonLayout->addWidget(cb.backButton);
    cb.buttonLayout->addWidget(cb.createButton);

    // Entities
    cb.entityLayout = new QVBoxLayout();
    cb.entityLayout->setMargin(0);
    cb.entityLayout->setSpacing(0);

    cb.entityLayout->addWidget(cb.titleLabel);
    cb.entityLayout->addWidget(cb.subtitleLabel);

    cb.entityLayout->addWidget(cb.projectName);
    cb.entityLayout->addWidget(cb.location);
    cb.entityLayout->addWidget(cb.solution);

    cb.upperLayout = new QHBoxLayout();
    cb.upperLayout->setMargin(0);
    cb.upperLayout->setSpacing(0);

    cb.upperLayout->addLayout(cb.entityLayout);
    cb.upperLayout->addStretch();

    cb.upperLayout->setStretch(0, 3);
    cb.upperLayout->setStretch(1, 1);

    cb.mainLayout = new QVBoxLayout();
    cb.mainLayout->setMargin(0);
    cb.mainLayout->setSpacing(0);

    cb.mainLayout->addLayout(cb.upperLayout);
    cb.mainLayout->addStretch();
    cb.mainLayout->addLayout(cb.buttonLayout);

    cb.w->setLayout(cb.mainLayout);

    q->connect(cb.backButton, &QPushButton::clicked, q, &HomeWindow::_q_cancelCreate);
    q->connect(cb.createButton, &QPushButton::clicked, q, &HomeWindow::_q_confirmCreate);
}

void HomeWindowPrivate::cb_switchIn() {
    Q_Q(HomeWindow);

    if (!cb.w) {
        cb_init();
        cb_reloadStrings();
    }
    cb_clear();

    q->takeCentralWidget();
    w->hide();
    w->setParent(q);

    q->setCentralWidget(cb.w);
    cb.w->show();
}

void HomeWindowPrivate::cb_switchOut() {
    Q_Q(HomeWindow);

    q->takeCentralWidget();
    cb.w->hide();
    cb.w->setParent(q);

    q->setCentralWidget(w);
    w->show();
}

void HomeWindowPrivate::cb_clear() {
    cb.projectName->text->setText(HomeWindow::tr("LVProject1"));
    cb.location->text->setText(QDir::toNativeSeparators(qApp->desktopDir()));
    cb.solution->text->setText(HomeWindow::tr("LVProject1"));
}

void HomeWindowPrivate::cb_reloadStrings() {
    cb.titleLabel->setText(HomeWindow::tr("Configure New Project"));
    cb.subtitleLabel->setText(HomeWindow::tr("Empty Template"));

    cb.projectName->label->setText(HomeWindow::tr("Project Name"));
    cb.location->label->setText(HomeWindow::tr("Location"));
    cb.location->browseButton->setText(HomeWindow::tr("..."));
    cb.solution->label->setText(HomeWindow::tr("Definition File Name"));

    cb.backButton->setText(HomeWindow::tr("Back"));
    cb.createButton->setText(HomeWindow::tr("Create"));
}
