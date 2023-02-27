#include "VExplorerPanel.h"

// https://vicrucann.github.io/tutorials/qtreewidget-child-drag-notify/

VExplorerPanel::VExplorerPanel(QWidget *parent) : QFrame(parent) {
    // Items
    itemsLabel = new QLabel();
    itemsLabel->setObjectName("items-label");

    itemsSearchBox = new QLineEdit();
    itemsSearchBox->setObjectName("items-search-box");

    itemsTree = new VSpkTreeWidget();
    itemsTree->setObjectName("speakers-tree");

    itemsTree->setEditTriggers(QTreeWidget::DoubleClicked);
    itemsTree->setDragEnabled(true);
    itemsTree->showDropIndicator();
    itemsTree->setDragDropMode(QTreeWidget::InternalMove);

    itemsLayout = new QVBoxLayout();
    itemsLayout->setMargin(0);
    itemsLayout->setSpacing(0);

    itemsLayout->addWidget(itemsLabel);
    itemsLayout->addWidget(itemsSearchBox);
    itemsLayout->addWidget(itemsTree);

    itemsWidget = new QFrame();
    itemsWidget->setObjectName("explorer-items-widget");
    itemsWidget->setLayout(itemsLayout);

    // Slices
    slicesLabel = new QLabel();
    slicesLabel->setObjectName("slices-label");

    slicesSearchBox = new QLineEdit();
    slicesSearchBox->setObjectName("slices-search-box");

    slicesList = new VSliceListWidget();
    slicesList->setObjectName("slices-list");

    splitter = new QSplitter();
    splitter->setObjectName("splitter");

    slicesLayout = new QVBoxLayout();
    slicesLayout->setMargin(0);
    slicesLayout->setSpacing(0);

    slicesLayout->addWidget(slicesLabel);
    slicesLayout->addWidget(slicesSearchBox);
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
    slicesSearchBox->setPlaceholderText(tr("Search Slices"));
}

void VExplorerPanel::setSpeakers(const QList<PianoSpec::SpeakerDesc> &speakers) {
    for (const auto &desc : speakers) {
        auto item = new QTreeWidgetItem(RootSpeaker);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setFlags(item->flags() & ~Qt::ItemIsDragEnabled);

        // Properties
        item->setData(0, LVSpeaker_ID, desc.id);
        item->setData(0, LVSpeaker_Name, desc.name);

        // Display
        item->setText(0, desc.name);
        itemsTree->addTopLevelItem(item);

        // Save
        auto node = new TreeNode{desc.id, item, {}, {}};
        speakerNodes.insert(desc.id, QSharedPointer<TreeNode>(node)); // Key is Speaker ID
        itemsTree->addTopLevelItem(item);
    }
}

void VExplorerPanel::setItems(const QList<QPair<QString, PianoSpec::ItemDesc>> &items) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        QStringList pathOrder = it->first.isEmpty() ? QStringList() : it->first.split("/");
        const PianoSpec::ItemDesc &desc = it->second;
        TreeNode *curNode = nullptr;

        // Find Speaker
        auto it2 = speakerNodes.find(desc.speaker); // Search by Speaker ID
        if (it2 == speakerNodes.end()) {
            continue;
        }
        curNode = it2.value().data();

        // Find Node To Insert
        for (const QString &dir : qAsConst(pathOrder)) {
            auto it3 = curNode->subdirs.find(dir);
            if (it3 == curNode->subdirs.end()) {
                // Insert Dir
                auto item = new QTreeWidgetItem(VirtualDir);
                item->setFlags(item->flags() | Qt::ItemIsEditable);

                // Display
                item->setText(0, dir);
                curNode->item->addChild(item);

                // Save
                auto node = new TreeNode{dir, item, {}, {}};
                curNode->subdirs.insert(dir, QSharedPointer<TreeNode>(node));

                curNode = node;
            } else {
                // Enter Inner Node And Insert Later
                curNode = it3.value().data();
            }
        }

        if (!desc.isDir) {
            // Insert File
            auto item = new QTreeWidgetItem(LVItem);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled);

            // Properties
            item->setData(0, LVItem_ID, desc.id);
            item->setData(0, LVItem_Name, desc.name);
            item->setData(0, LVItem_Speaker, desc.speaker);
            item->setData(0, LVItem_LangName, desc.lang.shortname);
            item->setData(0, LVItem_LangColor, desc.lang.color);

            // Display
            item->setText(0, desc.name);
            curNode->item->addChild(item);

            // Save
            curNode->files.insert(desc.id, item);
        }
    }
}

