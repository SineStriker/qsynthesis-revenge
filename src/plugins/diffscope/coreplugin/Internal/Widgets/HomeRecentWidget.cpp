#include "HomeRecentWidget.h"

#include "QsFrameworkNamespace.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>

#include <QMDecoratorV2.h>
#include <QMSvg.h>
#include <QMSystem.h>

#include <CLineEdit.h>
#include <CMenu.h>

#include "ICore.h"

namespace Core::Internal {

    static const char dateFormat[] = "yyyy-MM-dd hh:mm";

    class HomeRecentTopButtonBar : public IButtonBar {
    public:
        HomeRecentTopFrame *frame;

        explicit HomeRecentTopButtonBar(HomeRecentTopFrame *frame) : frame(frame) {
            setParent(frame);
        }

        QAbstractButton *addButton(const QString &id) override {
            return frame->addButton(id);
        }

        void removeButton(const QString &id) override {
            frame->removeButton(id);
        }
    };

    /**
     * @brief Recent widget top frame
     */
    HomeRecentTopFrame::HomeRecentTopFrame(QWidget *parent) : QFrame(parent) {
        searchBox = new CLineEdit();
        searchBox->setClearButtonEnabled(true);
        searchBox->setObjectName("search-box");

        ICore::autoPolishPopupMenu(searchBox);

        newButton = new CTabButton();
        newButton->setProperty("type", "top-button");
        newButton->setObjectName("new-button");
        newButton->setIcon(QMSvg::create(":/svg/home/new-file.svg"));

        openButton = new CTabButton();
        openButton->setProperty("type", "top-button");
        openButton->setObjectName("open-button");
        openButton->setIcon(QMSvg::create(":/svg/home/open-file.svg"));

        topLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        topLayout->setMargin(0);
        topLayout->setSpacing(0);

        topLayout->addWidget(searchBox);
        topLayout->addWidgetE(newButton);
        topLayout->addWidgetE(openButton);

        setLayout(topLayout);

        qIDec->installLocale(this, _LOC(HomeRecentTopFrame, this));

        connect(newButton, &QAbstractButton::clicked, this, &HomeRecentTopFrame::newRequested);
        connect(openButton, &QAbstractButton::clicked, this, &HomeRecentTopFrame::openRequested);
        connect(searchBox, &QLineEdit::textChanged, this, &HomeRecentTopFrame::textChanged);

        // init interface
        m_buttonBar = new HomeRecentTopButtonBar(this);
    }

    HomeRecentTopFrame::~HomeRecentTopFrame() {
    }

    void HomeRecentTopFrame::reloadStrings() {
        newButton->setText(tr("New"));
        openButton->setText(tr("Open"));
        searchBox->setPlaceholderText(tr("Search for files"));
    }

    QAbstractButton *HomeRecentTopFrame::addButton(const QString &id) {
        if (externButtons.contains(id)) {
            return nullptr;
        }
        auto btn = new CTabButton();
        btn->setProperty("type", "top-button");
        topLayout->addWidgetE(btn);
        externButtons.insert(id, btn);
        return btn;
    }

    void HomeRecentTopFrame::removeButton(const QString &id) {
        auto btn = externButtons.value(id, nullptr);
        if (btn) {
            btn->deleteLater();
            externButtons.remove(id);
        }
    }

    IButtonBar *HomeRecentTopFrame::buttonBar() const {
        return m_buttonBar;
    }

    /**
     * @brief Recent widget bottom frame
     */

    HomeRecentBottomFrame::HomeRecentBottomFrame(QWidget *parent) : QFrame(parent) {
        fileWidget = new QsApi::TitleListWidget();
        ICore::autoPolishScrollArea(fileWidget);

        fileWidget->setObjectName("home-file-widget");
        fileWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        emptyLabel = new QLabel();
        emptyLabel->setObjectName("empty-label");

        bottomLayout = new QVBoxLayout();
        bottomLayout->setMargin(0);
        bottomLayout->setSpacing(0);

        bottomLayout->addWidget(fileWidget);
        bottomLayout->addWidget(emptyLabel);
        bottomLayout->addStretch();

        bottomLayout->setStretchFactor(fileWidget, 1);

        setLayout(bottomLayout);

        auto docMgr = ICore::instance()->documentSystem();
        connect(docMgr, &DocumentSystem::recentFilesChanged, this, &HomeRecentBottomFrame::_q_recentFilesChanged);
        connect(fileWidget, &QsApi::TitleListWidget::itemClickedEx, this, &HomeRecentBottomFrame::_q_itemClickedEx);

        // Reload recent files once
        reloadRecentFiles();

        qIDec->installLocale(this, _LOC(HomeRecentBottomFrame, this));
    }

    HomeRecentBottomFrame::~HomeRecentBottomFrame() {
    }

    void HomeRecentBottomFrame::reloadStrings() {
        updateEmptyLabel();
    }

    void HomeRecentBottomFrame::reloadRecentFiles() {
        auto docMgr = ICore::instance()->documentSystem();

        fileWidget->clear();

        int cnt = 0;
        for (const auto &fileName : docMgr->recentFiles()) {
            QFileInfo info(fileName);
            auto spec = docMgr->supportedDocType(info.completeSuffix());

            auto item = new QListWidgetItem();
            item->setData(QsApi::DecorationRole, spec ? spec->icon() : QIcon());
            item->setData(QsApi::DisplayRole, QDir::toNativeSeparators(info.absoluteFilePath()));
            item->setData(QsApi::SubtitleRole, QDir::toNativeSeparators(info.absolutePath()));
            item->setData(QsApi::DescriptionRole, info.lastModified().toString(dateFormat));

            fileWidget->addItem(item);

            if (++cnt > 20) {
                break;
            }
        }
        // fileWidget->resize(fileWidget->width(), fileWidget->contentsSize().height());
        updateListFilter();
    }

    void HomeRecentBottomFrame::setFilterKeyword(const QString &keyword) {
        m_keyword = keyword;
        m_keyword.replace("\\", "/");
        updateListFilter();
    }

    void HomeRecentBottomFrame::updateListFilter() {
        int cnt = 0;
        for (int i = 0; i < fileWidget->count(); ++i) {
            auto item = fileWidget->item(i);

            QString filename = item->data(Qt::DisplayRole).toString();
            bool visible = m_keyword.isEmpty() || filename.contains(m_keyword, Qt::CaseInsensitive);
            item->setHidden(!visible);

            if (visible) {
                cnt++;
            }
        }

        if (fileWidget->count() == 0 || cnt == 0) {
            fileWidget->hide();
            emptyLabel->show();
            updateEmptyLabel();
        } else {
            fileWidget->show();
            emptyLabel->hide();
        }
    }

    void HomeRecentBottomFrame::updateEmptyLabel() {
        emptyLabel->setText(fileWidget->count() == 0 ? tr("No data.") : tr("Nothing to show."));
    }

    void HomeRecentBottomFrame::_q_recentFilesChanged() {
        reloadRecentFiles();
    }

    void HomeRecentBottomFrame::_q_itemClickedEx(const QModelIndex &index, int button) {
        int type = QDir::Files;
        QString filename = index.data(Qt::DisplayRole).toString();
        if (button == Qt::LeftButton) {
            if (type == QDir::Files) {
                emit openFileRequested(filename);
            } else {
                //
            }
        } else if (button == Qt::RightButton) {
            auto &menu = *ICore::createCoreMenu(this);

            QAction openAction(tr("Open(&O)"));
            // QAction newWinAction(tr("Open in new window(&E)"));
            QAction removeAction(tr("Remove from list(&R)"));
            QAction revealAction;

            menu.addAction(&openAction);
            // menu.addAction(&newWinAction);
            menu.addAction(&removeAction);

            if (type == QDir::Files) {
                revealAction.setText(tr("Show in %1(&S)").arg(QMOs::fileManagerName()));
                menu.addSeparator();
                menu.addAction(&revealAction);
            } else if (type == QDir::Dirs) {
                revealAction.setText(tr("Open in %1(&S)").arg(QMOs::fileManagerName()));
                menu.addSeparator();
                menu.addAction(&revealAction);
            }

            QAction *action = menu.exec(QCursor::pos());
            if (action == &openAction) {
                if (type == QDir::Files) {
                    emit openFileRequested(filename);
                } else {
                    //
                }
            }
            // else if (action == &newWinAction) {
            //     qWindow->newWindow({filename}, qWindow);
            // }
            else if (action == &removeAction) {
                ICore::instance()->documentSystem()->removeRecentFile(filename);
            } else if (action == &revealAction) {
                QMFs::reveal(filename);
            }

            delete &menu;
        }
    }

    /**
     * @brief Recent widget
     */
    HomeRecentWidget::HomeRecentWidget(QWidget *parent) : QSplitter(Qt::Vertical, parent) {
        topWidget = new HomeRecentTopFrame();
        topWidget->setObjectName("top-widget");
        topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        bottomWidget = new HomeRecentBottomFrame();
        bottomWidget->setObjectName("home-recent-list-container");
        bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        setChildrenCollapsible(false);

        addWidget(topWidget);
        addWidget(bottomWidget);

        setStretchFactor(0, 0);
        setStretchFactor(1, 1);

        qIDec->installLocale(this, _LOC(HomeRecentWidget, this));

        connect(topWidget, &HomeRecentTopFrame::textChanged, this, &HomeRecentWidget::_q_searchTextChanged);
    }

    HomeRecentWidget::~HomeRecentWidget() {
    }

    void HomeRecentWidget::reloadStrings() {
    }

    void HomeRecentWidget::_q_searchTextChanged(const QString &text) {
        bottomWidget->setFilterKeyword(text);
    }

} // Core