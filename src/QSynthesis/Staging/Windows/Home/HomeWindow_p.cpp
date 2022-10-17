#include "HomeWindow_p.h"

#include <QCoreApplication>

HomeWindowPrivate::HomeWindowPrivate() {
}

HomeWindowPrivate::~HomeWindowPrivate() {
}

void HomeWindowPrivate::init() {
    Q_Q(HomeWindow);

    w = new QWidget();
    q->setCentralWidget(w);

    searchBox = new QLineEdit();
    newButton = new QPushButton();
    openButton = new QPushButton();

    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->setSpacing(0);

    topLayout->addWidget(searchBox);
    topLayout->addWidget(newButton);
    topLayout->addWidget(openButton);

    fileList = new QListWidget();

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(fileList);

    w->setLayout(mainLayout);

    q->connect(searchBox, &QLineEdit::textChanged, q, &HomeWindow::_q_searchBoxChanged);
    q->connect(newButton, &QPushButton::clicked, q, &HomeWindow::_q_newButtonClicked);
    q->connect(openButton, &QPushButton::clicked, q, &HomeWindow::_q_openButtonClicked);
}

void HomeWindowPrivate::reloadStrings_helper() {
    Q_Q(HomeWindow);

    searchBox->setPlaceholderText(HomeWindow::tr("Search Files"));
    newButton->setText(HomeWindow::tr("New"));
    openButton->setText(HomeWindow::tr("Open"));

    q->setWindowTitle(HomeWindow::tr("Welcome to %1").arg(qApp->applicationName()));
}
