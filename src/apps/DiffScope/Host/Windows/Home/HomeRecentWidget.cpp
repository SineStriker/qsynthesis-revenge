#include "HomeRecentWidget.h"

#include "QMDecorator.h"
#include "DsConsole.h"

#include "QsFileManager.h"
#include "WindowManager.h"

#include <QDir>
#include <QFileInfoList>

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

/**
 * @brief Recent widget top frame
 */
HomeRecentTopFrame::HomeRecentTopFrame(QWidget *parent) : QFrame(parent) {
    searchBox = new QLineEdit();
    searchBox->setObjectName("search-box");

    newButton = new CTabButton();
    newButton->setProperty("type", "top-button");
    newButton->setObjectName("new-button");
    newButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    openButton = new CTabButton();
    openButton->setProperty("type", "top-button");
    openButton->setObjectName("open-button");
    openButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    importButton = new CTabButton();
    importButton->setProperty("type", "top-button");
    importButton->setObjectName("import-button");
    importButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    topLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
    topLayout->setMargin(0);
    topLayout->setSpacing(0);

    topLayout->addWidget(searchBox);
    topLayout->addWidgetE(newButton);
    topLayout->addWidgetE(openButton);
    topLayout->addWidgetE(importButton);

    setLayout(topLayout);

    connect(newButton, &QPushButton::clicked, this, &HomeRecentTopFrame::_q_newButtonClicked);
    connect(openButton, &QPushButton::clicked, this, &HomeRecentTopFrame::_q_openButtonClicked);
    connect(importButton, &QPushButton::clicked, this, &HomeRecentTopFrame::_q_importButtonClicked);

    qIDec->installLocale(this, {"DsHost"}, _LOC(HomeRecentTopFrame, this));
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
    qWindows->newProject();
}

void HomeRecentTopFrame::_q_openButtonClicked() {
    QDspxModel dspx;
    if (qCs->openFile(&dspx, this)) {
        qWindows->openProject(dspx);
    }
}

void HomeRecentTopFrame::_q_importButtonClicked() {
    QDspxModel dspx;
    if (qCs->importFile(&dspx, this)) {
        qWindows->openProject(dspx);
    }
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

    recentListWidget = new FileListWidget();
    recentListWidget->setObjectName("home-recent-list-widget");
    recentListWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    recentEmptyLabel = new QLabel();
    recentEmptyLabel->setObjectName("home-recent-empty-label");
    recentEmptyLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    recentContainerLayout = new QVBoxLayout();
    recentContainerLayout->setMargin(0);
    recentContainerLayout->setSpacing(0);
    recentContainerLayout->addWidget(recentListWidget);
    recentContainerLayout->addWidget(recentEmptyLabel);

    recentContainer = new QFrame();
    recentContainer->setObjectName("home-recent-list-container");
    recentContainer->setLayout(recentContainerLayout);
    bottomWidget->setWidget(recentContainer);

    recentListWidgetResizer = new QMAutoResizer(recentContainer);

    setChildrenCollapsible(false);

    addWidget(topWidget);
    addWidget(bottomWidget);

    setStretchFactor(0, 0);
    setStretchFactor(1, 1);

    connect(topWidget->searchBox, &QLineEdit::textChanged, this,
            &HomeRecentWidget::_q_searchTextChanged);
    connect(recentListWidget, &FileListWidget::itemClickedEx, this,
            &HomeRecentWidget::_q_recentItemClicked);
    connect(QsFileManager::instance(), &QsFileManager::recentCommited, this,
            &HomeRecentWidget::_q_recentCommited);

    qIDec->installLocale(this, {"DsHost"}, _LOC(HomeRecentWidget, this));

    reloadRecentList();
}

HomeRecentWidget::~HomeRecentWidget() {
}

void HomeRecentWidget::reloadStrings() {
    recentEmptyLabel->setText(tr("No data."));
}

QTypeList HomeRecentWidget::styleData() const {
    return {QVariant::fromValue(icon), QVariant::fromValue(iconSize)};
}

void HomeRecentWidget::setStyleData(const QTypeList &value) {
    if (value.size() >= 2) {
        int i = 0;
        DECODE_STYLE(icon, value.at(i++), QIcon);
        DECODE_STYLE(iconSize, value.at(i++), QSize);
        reloadRecentList();
    }
    emit styleDataChanged();
}

void HomeRecentWidget::reloadRecentList() {
    recentListWidget->clear();
    QStringList files = QsFileManager::instance()->fetchRecent(QsFileManager::Project);
    for (const QString &file : qAsConst(files)) {
        QFileInfo info(file);
        recentListWidget->addItem(icon, iconSize, QsFileManager::Project,
                                  QDir::toNativeSeparators(info.fileName()),
                                  QDir::toNativeSeparators(info.absoluteFilePath()),
                                  info.lastModified().toString(DateFormat));
    }
    recentListWidget->setVisible(recentListWidget->count() != 0);
    recentEmptyLabel->setVisible(recentListWidget->count() == 0);
}

void HomeRecentWidget::_q_searchTextChanged(const QString &text) {
    qDebug() << text;
}

void HomeRecentWidget::_q_recentItemClicked(const QModelIndex &index, int button) {
    int type = index.data(FileListWidget::Type).toInt();
    QString filename = index.data(FileListWidget::Location).toString();
    if (button == Qt::LeftButton) {
        if (type == QsFileManager::Project) {
            QDspxModel dspx;
            if (qCs->openFile(&dspx, filename, this)) {
                qWindows->openProject(dspx);
            }
        } else {
            // Dir handle
        }
    } else {
        // Right click handle
    }
}

void HomeRecentWidget::_q_recentCommited() {
    reloadRecentList();
}
