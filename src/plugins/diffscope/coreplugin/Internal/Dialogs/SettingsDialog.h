#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>

#include <CTabButton.h>
#include <QMEqualBoxLayout.h>
#include <QPixelSize.h>

#include "QMConfigurableDialog.h"

namespace Core {

    class ISettingPage;

    namespace Internal {

        class SettingsDialog : public QMConfigurableDialog {
            Q_OBJECT
            Q_LAYOUT_PROPERTY_DELCARE
        public:
            explicit SettingsDialog(QWidget *parent = nullptr);
            ~SettingsDialog();

            void reloadStrings();

        public:
            void selectPage(const QString &id);

        protected:
            void apply() override;
            void finish() override;

        private:
            QTreeWidget *m_tree;
            QStackedWidget *m_page;
            QLineEdit *m_searchBox;

            QWidget *leftWidget;
            QVBoxLayout *leftLayout;

            QWidget *rightWidget;
            QVBoxLayout *rightLayout;

            QLabel *titleLabel;
            QLabel *descriptionLabel;
            QLayout *labelLayout;

            QSplitter *topSplitter;

            QHash<ISettingPage *, QTreeWidgetItem *> m_treeIndexes;
            ISettingPage *m_currentPage;
            QWidget *m_catalogWidget;

            QTreeWidgetItem *buildTreeWidgetItem(ISettingPage *page);

            void clearPage();
            void showCatalog();

        private:
            void _q_titleChanged(ISettingPage *page, const QString &title);
            void _q_descriptionChanged(ISettingPage *page, const QString &description);

            void _q_pageAdded(ISettingPage *page);
            void _q_pageRemoved(ISettingPage *page);

            void _q_currentItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev);
            void _q_searchBoxTextChanged(const QString &text);
        };

    }

}



#endif // SETTINGSDIALOG_H
