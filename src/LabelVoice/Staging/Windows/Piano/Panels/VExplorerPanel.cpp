#include "VExplorerPanel.h"

VExplorerPanel::VExplorerPanel(QWidget *parent) : QFrame(parent) {
    // Items
    itemsLabel = new QLabel();
    itemsLabel->setObjectName("items-label");

    itemsSearchBox = new QLineEdit();
    itemsSearchBox->setObjectName("items-search-box");

    speakersTree = new VSpeakerTreeWidget();
    speakersTree->setObjectName("speakers-tree");

    itemsLayout = new QVBoxLayout();
    itemsLayout->setMargin(0);
    itemsLayout->setSpacing(0);

    itemsLayout->addWidget(itemsLabel);
    itemsLayout->addWidget(itemsSearchBox);
    itemsLayout->addWidget(speakersTree);

    itemsWidget = new QFrame();
    itemsWidget->setObjectName("explorer-items-widget");
    itemsWidget->setLayout(itemsLayout);

    // Speakers
    slicesLabel = new QLabel();
    slicesLabel->setObjectName("slices-label");

    secsSearchBox = new QLineEdit();
    secsSearchBox->setObjectName("slices-search-box");

    slicesList = new VSliceListWidget();
    slicesList->setObjectName("slices-list");

    splitter = new QSplitter();
    splitter->setObjectName("splitter");

    slicesLayout = new QVBoxLayout();
    slicesLayout->setMargin(0);
    slicesLayout->setSpacing(0);

    slicesLayout->addWidget(slicesLabel);
    slicesLayout->addWidget(secsSearchBox);
    slicesLayout->addWidget(slicesList);

    slicesWidget = new QFrame();
    slicesWidget->setObjectName("explorer-slices-widget");
    slicesWidget->setLayout(slicesLayout);

    // Main
    splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(itemsWidget);
    splitter->addWidget(slicesWidget);

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(splitter);

    setLayout(mainLayout);
}

VExplorerPanel::~VExplorerPanel() {
}

void VExplorerPanel::reloadStrings() {
    itemsLabel->setText(tr("Items"));
    itemsSearchBox->setPlaceholderText(tr("Search Items"));

    slicesLabel->setText(tr("Slices"));
    secsSearchBox->setPlaceholderText(tr("Search Slices"));
}
