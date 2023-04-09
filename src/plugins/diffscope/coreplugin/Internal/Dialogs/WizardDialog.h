#ifndef CHORUSKIT_WIZARDDIALOG_H
#define CHORUSKIT_WIZARDDIALOG_H

#include "CoreControls/WorkflowDialog.h"

namespace Core {

    namespace Internal {

        class WizardDialog : public WorkflowDialog {
            Q_OBJECT
        public:
            explicit WizardDialog(QWidget *parent = nullptr);
            ~WizardDialog();

            void reloadStrings();

            int step;

        protected:
            bool prev() override;
            bool next() override;
            bool finish() override;

        private:
            void refreshPage();
        };

    }

}



#endif // CHORUSKIT_WIZARDDIALOG_H
