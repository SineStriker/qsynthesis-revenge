#ifndef CHORUSKIT_SETTINGSDIALOG_H
#define CHORUSKIT_SETTINGSDIALOG_H

#include <QBoxLayout>
#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>

#include <Layout/QMEqualBoxLayout.h>
#include <Metatypes/QPixelSize.h>
#include <Widgets/CTabButton.h>

#include "Dialogs/ConfigurableDialog.h"

namespace Core {

    class ISettingPage;

    namespace Internal {

        class SettingsDialog : public ConfigurableDialog {
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



#endif // CHORUSKIT_SETTINGSDIALOG_H
