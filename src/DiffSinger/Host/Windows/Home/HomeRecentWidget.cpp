#include "HomeRecentWidget.h"

#include "CDecorator.h"

/**
 * @brief Recent widget top frame
 */
HomeRecentTopFrame::HomeRecentTopFrame(QWidget *parent) : QFrame(parent) {
    searchBox = new QLineEdit();
    searchBox->setObjectName("search-box");

    newButton = new CTabButton();
    newButton->setProperty("type", "top-button");
    newButton->setObjectName("new-button");

    importButton = new CTabButton();
    importButton->setProperty("type", "top-button");
    importButton->setObjectName("import-button");

    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->setSpacing(0);

    topLayout->addWidget(searchBox);
    topLayout->addWidget(newButton);
    topLayout->addWidget(importButton);

    setLayout(topLayout);

    qIDec->installLocale(this, {"DsHost"}, _LOC(HomeRecentTopFrame, this));
}

HomeRecentTopFrame::~HomeRecentTopFrame() {
}

void HomeRecentTopFrame::reloadStrings() {
    newButton->setText(tr("New"));
    importButton->setText(tr("Import"));
    searchBox->setPlaceholderText(tr("Search for files"));
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
}

HomeRecentWidget::~HomeRecentWidget() {
}

void HomeRecentWidget::reloadStrings() {
}
