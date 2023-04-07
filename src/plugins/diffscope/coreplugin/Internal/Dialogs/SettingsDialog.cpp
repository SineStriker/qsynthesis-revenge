#include "SettingsDialog.h"

#include "ICore.h"

#include <QCollator>
#include <QLabel>

#include <QMDecorator.h>

namespace Core {

    static int TitleRole = Qt::UserRole + 1000;
    static int EntityRole = TitleRole + 1;

    namespace Internal {

        struct Order {
            QCollator qoc;
            Order() {
                qoc = QCollator(QLocale(QLocale::English));
            }

            bool operator()(QTreeWidgetItem *a, QTreeWidgetItem *b) const {
                return qoc.compare(a->data(0, TitleRole).toString(), b->data(0, TitleRole).toString()) < 0;
            }
        };

        SettingsDialog::SettingsDialog(QWidget *parent) : ConfigurableDialog(parent) {
            m_currentPage = nullptr;
            m_catalogWidget = nullptr;

            m_tree = new QTreeWidget();
            m_tree->setHeaderHidden(true);

            m_searchBox = new QLineEdit();

            leftLayout = new QVBoxLayout();
            leftLayout->setMargin(0);
            // leftLayout->setSpacing(0);

            leftLayout->addWidget(m_searchBox);
            leftLayout->addWidget(m_tree);

            leftWidget = new QWidget();
            leftWidget->setLayout(leftLayout);

            m_page = new QStackedWidget();

            topSplitter = new QSplitter();
            topSplitter->addWidget(leftWidget);
            topSplitter->addWidget(m_page);

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

            qIDec->installLocale(this, {{}}, _LOC(SettingsDialog, this));
        }

        SettingsDialog::~SettingsDialog() {
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
            item->setData(0, TitleRole, page->sortKeyword());
            item->setData(0, EntityRole, QVariant::fromValue(page));

            QList<QTreeWidgetItem *> items;
            for (auto sub : page->pages()) {
                items.append(buildTreeWidgetItem(sub));
            }
            std::sort(items.begin(), items.end(), Order());
            item->addChildren(items);

            m_treeIndexes.insert(page, item);
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
        }

        void SettingsDialog::showCatalog() {
            auto w = new QFrame();

            auto layout = new QVBoxLayout();

            auto label = new QLabel(m_currentPage->description());
            layout->addWidget(label);

            // Update text
            connect(m_currentPage, &ISettingPage::descriptionChanged, label, &QLabel::setText);

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
            m_page->addWidget(w);

            m_catalogWidget = w;
        }

        void SettingsDialog::_q_titleChanged(ISettingPage *page, const QString &title) {
            auto item = m_treeIndexes.value(page, nullptr);
            if (!item) {
                return;
            }
            item->setText(0, title);
        }

        void SettingsDialog::_q_descriptionChanged(Core::ISettingPage *page, const QString &description) {
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
                if (it.key()->matches(text)) {
                    items.push_back(it.value());
                }
            }

            // All hidden
            for (const auto &item : qAsConst(m_treeIndexes)) {
                item->setHidden(true);
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