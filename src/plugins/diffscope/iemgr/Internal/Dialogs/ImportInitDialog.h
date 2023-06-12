#ifndef IMPORTINITDIALOG_H
#define IMPORTINITDIALOG_H

#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <QMWorkflowDialog.h>

#include "IWizardFactory.h"

#include "CTabButton.h"

namespace IEMgr {

    namespace Internal {

        class ImportInitDialog : public QMWorkflowDialog {
            Q_OBJECT
        public:
            explicit ImportInitDialog(QWidget *parent = nullptr);
            ~ImportInitDialog();

        public:
            IWizardFactory *currentWizard() const;

            QString currentPath() const;
            void setCurrentPath(const QString &path);

            void selectWizard(IWizardFactory *wizard);

        protected:
            bool finish() override;

        private:
            QMWorkflowPage *page;

            QVBoxLayout *leftLayout;
            QVBoxLayout *rightLayout;

            QHash<QString, QLabel *> labels;
            QButtonGroup *buttonGroup;

            QHBoxLayout *textButtonLayout;
            QLineEdit *lineEdit;
            QPushButton *browseButton;
            QLabel *pathLabel;
            QLabel *hintLabel;

            IWizardFactory *curWizard;

            void initWizardsDisplay();

        private:
            void _q_browseButtonClicked();
            void _q_itemButtonClicked();
            void _q_textChanged(const QString &text);
        };

    }

}


#endif // IMPORTINITDIALOG_H
