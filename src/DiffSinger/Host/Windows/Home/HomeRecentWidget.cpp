#include "HomeRecentWidget.h"

#include "CDecorator.h"
#include "DsConsole.h"

/**
 * @brief Recent widget top frame
 */
HomeRecentTopFrame::HomeRecentTopFrame(QWidget *parent) : QFrame(parent) {
    searchBox = new QLineEdit();
    searchBox->setObjectName("search-box");

    newButton = new CTabButton();
    newButton->setProperty("type", "top-button");
    newButton->setObjectName("new-button");

    openButton = new CTabButton();
    openButton->setProperty("type", "top-button");
    openButton->setObjectName("open-button");

    importButton = new CTabButton();
    importButton->setProperty("type", "top-button");
    importButton->setObjectName("import-button");

    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->setSpacing(0);

    topLayout->addWidget(searchBox);
    topLayout->addWidget(newButton);
    topLayout->addWidget(openButton);
    topLayout->addWidget(importButton);

    setLayout(topLayout);

    qIDec->installLocale(this, {"DsHost"}, _LOC(HomeRecentTopFrame, this));

    connect(newButton, &QPushButton::clicked, this, &HomeRecentTopFrame::_q_newButtonClicked);
    connect(openButton, &QPushButton::clicked, this, &HomeRecentTopFrame::_q_openButtonClicked);
    connect(importButton, &QPushButton::clicked, this, &HomeRecentTopFrame::_q_importButtonClicked);
}

HomeRecentTopFrame::~HomeRecentTopFrame() {
}

void HomeRecentTopFrame::reloadStrings() {
    newButton->setText(tr("New"));
    openButton->setText(tr("Open"));
    importButton->setText(tr("Import"));
    searchBox->setPlaceholderText(tr("Search for files"));
}

void HomeRecentTopFrame::_q_newButtonClicked() {
}

void HomeRecentTopFrame::_q_openButtonClicked() {
    QDspxModel dspx;
    qCs->openFile(&dspx, this);
}

void HomeRecentTopFrame::_q_importButtonClicked() {
    QDspxModel dspx;
    qCs->importFile(&dspx, this);
}

/**
 * @brief Recent widget
 */
HomeRecentWidget::HomeRecentWidget(QWidget *parent) : QSplitter(Qt::Vertical, parent) {
    topWidget = new HomeRecentTopFrame();
    topWidget->setObjectName("top-widget");
    topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    bottomWidget = new LinearScrollArea(Qt::Vertical);
    bottomWidget->setObjectName("bottom-widget");
    bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    setChildrenCollapsible(false);

    addWidget(topWidget);
    addWidget(bottomWidget);

    setStretchFactor(0, 0);
    setStretchFactor(1, 1);

    qIDec->installLocale(this, {"DsHost"}, _LOC(HomeRecentWidget, this));

    connect(topWidget->searchBox, &QLineEdit::textChanged, this,
            &HomeRecentWidget::_q_searchTextChanged);
}

HomeRecentWidget::~HomeRecentWidget() {
}

void HomeRecentWidget::reloadStrings() {
}

void HomeRecentWidget::_q_searchTextChanged(const QString &text) {
    qDebug() << text;
}
