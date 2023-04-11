#ifndef USTWIZARD_H
#define USTWIZARD_H

#include "iemgr/IWizardFactory.h"

namespace IEMgr {

    namespace Internal {

        class UstWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit UstWizard(QObject *parent = nullptr);
            ~UstWizard();

        public:
            Features features() const override;
            QString filter(Feature feature) const override;
            bool runWizard(Feature feature, IWizardContext *context) override;
        };
    }
} // IEMgr

#endif // USTWIZARD_H
