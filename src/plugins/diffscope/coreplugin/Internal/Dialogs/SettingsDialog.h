#ifndef CHORUSKIT_SettingsDialog_H
#define CHORUSKIT_SettingsDialog_H

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

            QTreeWidgetItem *buildTreeWidgetItem(ISettingPage *page);

        private:
            void _q_titleChanged(ISettingPage *page, const QString &title);

            void _q_pageAdded(ISettingPage *page);
            void _q_pageRemoved(ISettingPage *page);

            void _q_currentItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev);
        };

    }

}



#endif // CHORUSKIT_SettingsDialog_H
