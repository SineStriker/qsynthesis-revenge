#include "SettingsDialog.h"

#include "CoreApi/ILoader.h"
#include "ICore.h"

#include <QCollator>
#include <QLabel>

#include <CLineEdit.h>
#include <QMDecoratorV2.h>

namespace Core {

    static int TitleRole = Qt::UserRole + 1000;
    static int EntityRole = TitleRole + 1;

    static const char settingCategoryC[] = "Core/SettingDialog";

    namespace Internal {

        static QString _getItemPathTitle(QTreeWidgetItem *item) {
            QStringList titles;
            while (item) {
                titles.prepend(item->text(0));
                item = item->parent();
            }
            return titles.join(" > ");
        }

        struct Order {
            QCollator qoc;
            Order() {
                qoc = QCollator(QLocale(QLocale::English));
            }

            bool operator()(QTreeWidgetItem *a, QTreeWidgetItem *b) const {
                return qoc.compare(a->data(0, TitleRole).toString(), b->data(0, TitleRole).toString()) < 0;
            }
        };

        SettingsDialog::SettingsDialog(QWidget *parent) : QMConfigurableDialog(parent) {
            m_currentPage = nullptr;
            m_catalogWidget = nullptr;

            // Left
            m_tree = new QTreeWidget();
            m_tree->setHeaderHidden(true);

            m_searchBox = new CLineEdit();
            m_searchBox->setClearButtonEnabled(true);

            leftLayout = new QVBoxLayout();
            leftLayout->setMargin(0);
            // leftLayout->setSpacing(0);

            leftLayout->addWidget(m_searchBox);
            leftLayout->addWidget(m_tree);

            leftWidget = new QWidget();
            leftWidget->setLayout(leftLayout);

            // Right
            titleLabel = new QLabel();
            descriptionLabel = new QLabel();

            labelLayout = new QVBoxLayout();
            labelLayout->setMargin(0);
            labelLayout->setSpacing(0);

            labelLayout->addWidget(titleLabel);
            labelLayout->addWidget(descriptionLabel);

            m_page = new QStackedWidget();

            rightLayout = new QVBoxLayout();
            rightLayout->setMargin(0);
            // leftLayout->setSpacing(0);

            rightLayout->addLayout(labelLayout);
            rightLayout->addWidget(m_page);

            rightWidget = new QWidget();
            rightWidget->setLayout(rightLayout);

            topSplitter = new QSplitter();
            topSplitter->addWidget(leftWidget);
            topSplitter->addWidget(rightWidget);

            topSplitter->setStretchFactor(0, 0);
            topSplitter->setStretchFactor(1, 1);

            setWidget(topSplitter);

            auto sc = ICore::instance()->settingCatalog();

            // Build tree widget
            {
                QList<QTreeWidgetItem *> items;
                for (auto page : sc->pages()) {
                    items.append(buildTreeWidgetItem(page));
                }
                std::sort(items.begin(), items.end(), Order());
                m_tree->addTopLevelItems(items);
            }

            connect(m_tree, &QTreeWidget::currentItemChanged, this, &SettingsDialog::_q_currentItemChanged);
            connect(m_searchBox, &QLineEdit::textChanged, this, &SettingsDialog::_q_searchBoxTextChanged);

            connect(sc, &SettingCatalog::titleChanged, this, &SettingsDialog::_q_titleChanged);
            connect(sc, &SettingCatalog::descriptionChanged, this, &SettingsDialog::_q_descriptionChanged);
            connect(sc, &SettingCatalog::pageAdded, this, &SettingsDialog::_q_pageAdded);
            connect(sc, &SettingCatalog::pageRemoved, this, &SettingsDialog::_q_pageRemoved);

            qIDec->installLocale(this, _LOC(SettingsDialog, this));

            // Init window sizes
            auto winMgr = ICore::instance()->windowSystem();
            winMgr->loadWindowGeometry(metaObject()->className(), this, {1280, 720});
            winMgr->loadSplitterSizes(metaObject()->className(), topSplitter, {250, width() - 250});
        }

        SettingsDialog::~SettingsDialog() {
            // Save window sizes
            auto winMgr = ICore::instance()->windowSystem();
            winMgr->saveSplitterSizes(metaObject()->className(), topSplitter);
            winMgr->saveWindowGeometry(metaObject()->className(), this);
        }

        void SettingsDialog::reloadStrings() {
            m_searchBox->setPlaceholderText(tr("Search for settings"));

            setWindowTitle(tr("Settings"));
        }

        void SettingsDialog::selectPage(const QString &id) {
            auto sc = ICore::instance()->settingCatalog();
            auto pages = sc->pages(id);
            if (!pages.isEmpty()) {
                auto item = m_treeIndexes.value(pages.front(), nullptr);
                if (item) {
                    m_tree->setCurrentItem(item);
                    return;
                }
            }
            if (m_tree->topLevelItemCount()) {
                m_tree->setCurrentItem(m_tree->topLevelItem(0));
                return;
            }

            clearPage();
        }

        void SettingsDialog::apply() {
            // accept all
            for (auto it = m_treeIndexes.begin(); it != m_treeIndexes.end(); ++it) {
                it.key()->accept();
            }
        }

        void SettingsDialog::finish() {
            // finish all
            for (auto it = m_treeIndexes.begin(); it != m_treeIndexes.end(); ++it) {
                it.key()->finish();
            }

            takeWidget();
        }

        QTreeWidgetItem *SettingsDialog::buildTreeWidgetItem(Core::ISettingPage *page) {
            auto item = new QTreeWidgetItem();
            item->setText(0, page->title());
            item->setToolTip(0, page->description());
            item->setData(0, TitleRole, page->sortKeyword());
            item->setData(0, EntityRole, QVariant::fromValue(page));

            QList<QTreeWidgetItem *> items;
            for (auto sub : page->pages()) {
                items.append(buildTreeWidgetItem(sub));
            }
            std::sort(items.begin(), items.end(), Order());
            item->addChildren(items);

            m_treeIndexes.insert(page, item);

            auto text = m_searchBox->text();
            if (!text.isEmpty()) {
                item->setHidden(!page->matches(text));

                std::list<QTreeWidgetItem *> items;
                for (const auto &sub : page->allPages()) {
                    auto item = m_treeIndexes[sub];

                    // All hidden
                    item->setHidden(true);

                    if (sub->matches(text)) {
                        items.push_back(item);
                    }
                }

                for (auto p : qAsConst(items)) {
                    while (p && p->isHidden()) {
                        p->setHidden(false);
                        p = p->parent();
                    }
                }
            }

            return item;
        }

        void SettingsDialog::clearPage() {
            if (m_page->count()) {
                auto w = m_page->widget(0);
                m_page->removeWidget(w);

                if (w == m_catalogWidget) {
                    delete w;
                    m_catalogWidget = nullptr;
                }
            }

            titleLabel->setText({});
            descriptionLabel->setText({});
        }

        void SettingsDialog::showCatalog() {
            auto w = new QFrame();

            auto layout = new QVBoxLayout();

            auto curItem = m_tree->currentItem();
            for (int i = 0; i < curItem->childCount(); ++i) {
                auto page = curItem->child(i)->data(0, EntityRole).value<ISettingPage *>();
                if (!page)
                    continue;

                auto btn = new CTabButton();
                btn->setProperty("id", page->id());
                btn->setText(page->title());

                // Update text
                connect(page, &ISettingPage::titleChanged, btn, &QAbstractButton::setText);

                connect(btn, &QAbstractButton::clicked, this, [this]() {
                    selectPage(sender()->property("id").toString()); //
                });
                layout->addWidget(btn);
            }
            layout->addStretch();

            w->setLayout(layout);

            clearPage();

            titleLabel->setText(_getItemPathTitle(curItem));
            descriptionLabel->setText(m_currentPage->description());
            m_page->addWidget(w);

            m_catalogWidget = w;
        }

        void SettingsDialog::_q_titleChanged(ISettingPage *page, const QString &title) {
            auto item = m_treeIndexes.value(page, nullptr);
            if (!item) {
                return;
            }
            item->setText(0, title);

            auto curItem = m_tree->currentItem();
            while (item && item != curItem) {
                item = item->parent();
            }

            if (item) {
                titleLabel->setText(_getItemPathTitle(curItem));
            }
        }

        void SettingsDialog::_q_descriptionChanged(Core::ISettingPage *page, const QString &description) {
            auto item = m_treeIndexes.value(page, nullptr);
            if (!item) {
                return;
            }
            item->setToolTip(0, description);

            if (page == m_currentPage) {
                descriptionLabel->setText(description);
            }
        }

        void SettingsDialog::_q_pageAdded(ISettingPage *page) {
            auto sc = ICore::instance()->settingCatalog();

            auto item = buildTreeWidgetItem(page);
            auto parent = page->parent();

            Order o;
            if (parent == sc) {
                int i = 0;
                for (; i < m_tree->topLevelItemCount(); ++i) {
                    if (!o(m_tree->topLevelItem(i), item)) {
                        break;
                    }
                }
                m_tree->insertTopLevelItem(i, item);
            } else {
                auto parentItem = m_treeIndexes.value(qobject_cast<ISettingPage *>(parent), nullptr);
                if (!parentItem) {
                    return;
                }

                int i = 0;
                for (; i < parentItem->childCount(); ++i) {
                    if (!o(parentItem->child(i), item)) {
                        break;
                    }
                }
                parentItem->insertChild(i, item);
            }

            if (parent == m_currentPage && m_catalogWidget) {
                showCatalog();
            }
        }

        void SettingsDialog::_q_pageRemoved(ISettingPage *page) {
            auto item = m_treeIndexes.value(page, nullptr);
            if (!item) {
                return;
            }

            for (const auto &sub : page->allPages()) {
                m_treeIndexes.remove(sub);
            }
            m_treeIndexes.remove(page);
            delete item;

            if (page == m_currentPage) {
                selectPage(QString());
            } else if (page->parent() == m_currentPage && m_catalogWidget) {
                showCatalog();
            }
        }

        void SettingsDialog::_q_currentItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev) {
            Q_UNUSED(prev);

            clearPage();

            auto page = cur ? cur->data(0, EntityRole).value<ISettingPage *>() : nullptr;
            m_currentPage = page;

            if (!page) {
                return;
            }

            auto w = page->widget();
            if (w) {
                titleLabel->setText(_getItemPathTitle(cur));
                descriptionLabel->setText(page->description());
                m_page->addWidget(page->widget());
            } else {
                // Add a catalog widget with description
                showCatalog();
            }
        }

        void SettingsDialog::_q_searchBoxTextChanged(const QString &text) {
            if (text.isEmpty()) {
                for (const auto &item : qAsConst(m_treeIndexes)) {
                    item->setHidden(false);
                }
                return;
            }

            std::list<QTreeWidgetItem *> items;
            for (auto it = m_treeIndexes.begin(); it != m_treeIndexes.end(); ++it) {
                auto item = it.value();

                // All hidden
                item->setHidden(true);

                if (it.key()->matches(text)) {
                    items.push_back(item);
                }
            }


            for (auto p : qAsConst(items)) {
                while (p && p->isHidden()) {
                    p->setHidden(false);
                    p = p->parent();
                }
            }
        }

    }

}