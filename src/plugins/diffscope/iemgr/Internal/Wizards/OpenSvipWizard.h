#ifndef CHORUSKIT_OPENSVIPWIZARD_H
#define CHORUSKIT_OPENSVIPWIZARD_H

#include "iemgr/IWizardFactory.h"

namespace IEMgr {

    namespace Internal{

        class OpenSvipWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit OpenSvipWizard(QObject *parent = nullptr);
            ~OpenSvipWizard();

        public:
            Features features() const override;
            QString filter(Feature feature) const override;
            bool runWizard(Feature feature, IWizardContext *context) override;
        };

    }

}


#endif // CHORUSKIT_OPENSVIPWIZARD_H
