#ifndef CHORUSKIT_IMPORTINITDIALOG_H
#define CHORUSKIT_IMPORTINITDIALOG_H

#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <CoreControls/WorkflowDialog.h>

#include "IImportWizard.h"

#include "CTabButton.h"

namespace IEMgr {

    namespace Internal {

        class ImportInitDialog : public Core::WorkflowDialog {
            Q_OBJECT
        public:
            explicit ImportInitDialog(QWidget *parent = nullptr);
            ~ImportInitDialog();

        public:
            IImportWizard *currentWizard() const;
            void selectWizard(IImportWizard *wizard);

        protected:
            bool finish() override;

        private:
            Core::WorkflowPage *page;

            QVBoxLayout *leftLayout;
            QVBoxLayout *rightLayout;

            QHash<QString, QLabel *> labels;
            QButtonGroup *buttonGroup;

            QHBoxLayout *textButtonLayout;
            QLineEdit *lineEdit;
            QPushButton *browseButton;
            QLabel *pathLabel;
            QLabel *hintLabel;

            IImportWizard *curWizard;

            void initWizardsDisplay();

        private:
            void _q_browseButtonClicked();
            void _q_itemButtonClicked();
            void _q_textChanged(const QString &text);
        };

    }

}


#endif // CHORUSKIT_IMPORTINITDIALOG_H
