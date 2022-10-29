#include "HomeMainWidget.h"

#include "QPixelSize.h"
#include "QSvgUri.h"

#include "Kernel/LvApplication.h"
#include "Managers/FileManager.h"
#include "SystemHelper.h"

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

static const char DateFormat[] = "yyyy-MM-dd hh:mm";

HomeMainWidget::HomeMainWidget(ProjectCommonBlock *block, QWidget *parent)
    : QFrame(parent), block(block) {
    setFocusPolicy(Qt::ClickFocus);

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
    connect(recentList->delegate(), &FileListItemDelegate::clicked, this,
            &HomeMainWidget::_q_recentItemClicked);

    connect(FileManager::instance(), &FileManager::recentCommited, this,
            &HomeMainWidget::_q_recentCommited);

    reloadRecentList();
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
    CFG.cont = QString();                                                                                 \
    templateList->addFileItem(CFG.icon, CFG.iconSize, CFG.id, CFG.title, CFG.subtitle, CFG.cont);

    SET_AND_ADD(emptyItemConfig, tr("Empty Template"), tr("Create empty project for marking"),
                Empty);
    SET_AND_ADD(opencpopItemConfig, tr("Opencpop Template"),
                tr("Use Opencpop template for marking"), Opencpop);
    SET_AND_ADD(diffItemConfig, tr("DiffSinger Template"),
                tr("Use DiffSinger template for marking"), DiffSinger);
    SET_AND_ADD(openvpiItemConfig, tr("OpenVPI Template"), tr("Use OpenVPI template for marking"),
                OpenVPI);

#undef SET_AND_ADD
}

void HomeMainWidget::reloadRecentList() {
    recentList->clear();

    QStringList files = FileManager::instance()->fetchRecent(FileManager::Project);
    for (const QString &file : qAsConst(files)) {
        QFileInfo info(file);
        recentList->addFileItem(recentFileConfig.icon, recentFileConfig.iconSize,
                                FileManager::Project, QDir::toNativeSeparators(info.fileName()),
                                QDir::toNativeSeparators(info.absoluteFilePath()),
                                info.lastModified().toString(DateFormat));
    }
}

QTypeList HomeMainWidget::templateStyleData() const {
    return {
        QVariant::fromValue(QSvgUri()),       emptyItemConfig.iconSize.width(),
        emptyItemConfig.iconSize.height(), //
        QVariant::fromValue(QSvgUri()),       opencpopItemConfig.iconSize.width(),
        opencpopItemConfig.iconSize.height(), //
        QVariant::fromValue(QSvgUri()),       diffItemConfig.iconSize.width(),
        diffItemConfig.iconSize.height(), //
        QVariant::fromValue(QSvgUri()),       openvpiItemConfig.iconSize.width(),
        openvpiItemConfig.iconSize.height(), //
    };
}

void HomeMainWidget::setTemplateStyleData(const QTypeList &list) {
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

QTypeList HomeMainWidget::recentStyleData() const {
    return {
        QVariant::fromValue(QSvgUri()),     recentFileConfig.iconSize.width(),
        recentFileConfig.iconSize.height(), //
        QVariant::fromValue(QSvgUri()),     recentDirConfig.iconSize.width(),
        recentDirConfig.iconSize.height(), //
    };
}

void HomeMainWidget::setRecentStyleData(const QTypeList &list) {
    if (list.size() >= 6) {
        int i = 0;
        DECODE_STYLE(recentFileConfig.icon, list.at(i++), QSvgUri);
        DECODE_STYLE_SETTER(recentFileConfig.iconSize, list.at(i++), QPixelSize, setWidth);
        DECODE_STYLE_SETTER(recentFileConfig.iconSize, list.at(i++), QPixelSize, setHeight);

        DECODE_STYLE(recentDirConfig.icon, list.at(i++), QSvgUri);
        DECODE_STYLE_SETTER(recentDirConfig.iconSize, list.at(i++), QPixelSize, setWidth);
        DECODE_STYLE_SETTER(recentDirConfig.iconSize, list.at(i++), QPixelSize, setHeight);

        reloadRecentList();
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
}

void HomeMainWidget::_q_recentItemClicked(const QModelIndex &index, int button) {
    int type = index.data(FileListItemDelegate::Type).toInt();
    QString filename = index.data(FileListItemDelegate::Location).toString();
    if (button == Qt::LeftButton) {
        if (type == FileManager::Project) {
            emit openRequested(filename);
        } else {
            // Dir handle
        }
    } else {
        // Right click handle
    }
}

void HomeMainWidget::_q_recentCommited() {
    reloadRecentList();
};
