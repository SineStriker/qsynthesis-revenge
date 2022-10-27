#include "HomeMainWidget.h"

#include "QPixelSize.h"
#include "QSvgUri.h"

#include "Kernel/LvApplication.h"

#define DECODE_STYLE(VAR, VARIANT, TYPE)                                                           \
    {                                                                                              \
        QVariant var = VARIANT;                                                                    \
        if (var.convert(qMetaTypeId<TYPE>())) {                                                    \
            VAR = var.value<TYPE>();                                                               \
        }                                                                                          \
    }

#define DECODE_STYLE_SETTER(VAR, VARIANT, TYPE, SETTER)                                            \
    {                                                                                              \
        QVariant var = VARIANT;                                                                    \
        if (var.convert(qMetaTypeId<TYPE>())) {                                                    \
            VAR.SETTER(var.value<TYPE>());                                                         \
        }                                                                                          \
    }

HomeMainWidget::HomeMainWidget(QWidget *parent) : QFrame(parent) {
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

    setLayout(mainLayout);

    connect(searchBox, &QLineEdit::textChanged, this, &HomeMainWidget::_q_searchBoxChanged);
    connect(templateList->delegate(), &FileListItemDelegate::clicked, this,
            &HomeMainWidget::_q_templateItemClicked);
}

HomeMainWidget::~HomeMainWidget() {
}

void HomeMainWidget::reloadStrings() {
    titleLabel->setText(qApp->applicationName());
    subtitleLabel->setText(tr("Select a template to create a project"));
    searchBox->setPlaceholderText(tr("Search for recent projects"));
}

void HomeMainWidget::reloadTemplates() {
    templateList->clear();
    openButton->setText(tr("Open Project"));

#define SET_AND_ADD(CFG, TITLE, SUBTITLE, ID)                                                      \
    CFG.title = TITLE;                                                                             \
    CFG.subtitle = SUBTITLE;                                                                       \
    CFG.id = ID;                                                                                   \
    templateList->addFileItem(CFG.icon, CFG.iconSize, CFG.id, CFG.title, CFG.subtitle, CFG.cont);

    SET_AND_ADD(emptyItemConfig, ("Empty Template"), ("Create empty project for marking"), Empty);
    SET_AND_ADD(opencpopItemConfig, ("Opencpop Template"), ("Use Opencpop template for marking"),
                Opencpop);
    SET_AND_ADD(diffItemConfig, ("DiffSinger Template"), ("Use DiffSinger template for marking"),
                DiffSinger);
    SET_AND_ADD(openvpiItemConfig, ("OpenVPI Template"), ("Use OpenVPI template for marking"),
                OpenVPI);

#undef SET_AND_ADD
}

QTypeList HomeMainWidget::styleData() const {
    return {
        QVariant::fromValue(QSvgUri()),      emptyItemConfig.iconSize.width(),
        emptyItemConfig.iconSize.height(),   QVariant::fromValue(QSvgUri()),
        opencpopItemConfig.iconSize.width(), opencpopItemConfig.iconSize.height(),
        QVariant::fromValue(QSvgUri()),      diffItemConfig.iconSize.width(),
        diffItemConfig.iconSize.height(),    QVariant::fromValue(QSvgUri()),
        openvpiItemConfig.iconSize.width(),  openvpiItemConfig.iconSize.height(),
    };
}

void HomeMainWidget::setStyleData(const QTypeList &list) {
    if (list.size() >= 12) {
        int i = 0;
        DECODE_STYLE(emptyItemConfig.icon, list.at(i++), QSvgUri);
        DECODE_STYLE_SETTER(emptyItemConfig.iconSize, list.at(i++), QPixelSize, setWidth);
        DECODE_STYLE_SETTER(emptyItemConfig.iconSize, list.at(i++), QPixelSize, setHeight);

        DECODE_STYLE(opencpopItemConfig.icon, list.at(i++), QSvgUri);
        DECODE_STYLE_SETTER(opencpopItemConfig.iconSize, list.at(i++), QPixelSize, setWidth);
        DECODE_STYLE_SETTER(opencpopItemConfig.iconSize, list.at(i++), QPixelSize, setHeight);

        DECODE_STYLE(diffItemConfig.icon, list.at(i++), QSvgUri);
        DECODE_STYLE_SETTER(diffItemConfig.iconSize, list.at(i++), QPixelSize, setWidth);
        DECODE_STYLE_SETTER(diffItemConfig.iconSize, list.at(i++), QPixelSize, setHeight);

        DECODE_STYLE(openvpiItemConfig.icon, list.at(i++), QSvgUri);
        DECODE_STYLE_SETTER(openvpiItemConfig.iconSize, list.at(i++), QPixelSize, setWidth);
        DECODE_STYLE_SETTER(openvpiItemConfig.iconSize, list.at(i++), QPixelSize, setHeight);

        reloadTemplates();
    }
}

void HomeMainWidget::_q_searchBoxChanged(const QString &text) {
    qDebug() << text;
}

void HomeMainWidget::_q_templateItemClicked(const QModelIndex &index, int button) {
    int type = index.data(FileListItemDelegate::Type).toInt();
    if (button == Qt::LeftButton) {
        switch (type) {
            case Empty:
                emit newRequested(Empty);
                break;
            case Opencpop:
                break;
            case DiffSinger:
                break;
            case OpenVPI:
                break;
        }
    } else {
        // Right click handle
    }
};
