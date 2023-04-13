#ifndef CHORUSKIT_SVIP3WIZARD_H
#define CHORUSKIT_SVIP3WIZARD_H

#include "iemgr/IWizardFactory.h"

#include "Dspx/QDspxModel.h"

namespace IEMgr {

    namespace Internal {

        class Svip3Wizard : public IEMgr::IWizardFactory {
            Q_OBJECT
        public:
            explicit Svip3Wizard(QObject *parent = nullptr);
            ~Svip3Wizard();

        public:
            Features features() const override;
            QString filter(Feature feature) const override;
            bool runWizard(Feature feature, IWizardContext *context) override;
        };

    }

}


#endif // CHORUSKIT_SVIP3WIZARD_H
