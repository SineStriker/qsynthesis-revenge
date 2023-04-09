#ifndef CHORUSKIT_USTWIZARD_H
#define CHORUSKIT_USTWIZARD_H

#include "iemgr/IImportWizard.h"

namespace IEMgr {

    namespace Internal {

        class UstWizard : public IImportWizard {
            Q_OBJECT
        public:
            explicit UstWizard(QObject *parent = nullptr);
            ~UstWizard();

        public:
            QString filter() const override;
            bool runWizard(WizardContext *context) override;
        };
    }
} // IEMgr

#endif // CHORUSKIT_USTWIZARD_H
