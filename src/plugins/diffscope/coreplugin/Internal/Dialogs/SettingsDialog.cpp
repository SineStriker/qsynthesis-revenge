#include "SettingsDialog.h"

#include "ICore.h"

#include <QCollator>

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

            connect(sc, &SettingCatalog::titleChanged, this, &SettingsDialog::_q_titleChanged);
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
            }
        }

        void SettingsDialog::apply() {
            // accept all
            for (auto it = m_treeIndexes.begin(); it != m_treeIndexes.end(); ++it) {
                it.key()->accept();
            }

            ConfigurableDialog::apply();
        }

        void SettingsDialog::finish() {
            // finish all
            for (auto it = m_treeIndexes.begin(); it != m_treeIndexes.end(); ++it) {
                it.key()->finish();
            }

            ConfigurableDialog::finish();
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

        void SettingsDialog::_q_titleChanged(ISettingPage *page, const QString &title) {
            auto item = m_treeIndexes.value(page, nullptr);
            if (!item) {
                return;
            }
            item->setText(0, title);
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
        }

        void SettingsDialog::_q_currentItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev) {
            Q_UNUSED(prev);

            auto page = cur->data(0, EntityRole).value<ISettingPage *>();
            if (page) {
                if (m_page->count()) {
                    m_page->removeWidget(m_page->widget(0));
                }
                m_page->addWidget(page->widget());
            }
        }

    }

}