#ifndef CHORUSKIT_SVIPWIZARD_H
#define CHORUSKIT_SVIPWIZARD_H

#include "iemgr/IWizardFactory.h"

namespace IEMgr {

    namespace Internal {

        class SvipWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit SvipWizard(QObject *parent = nullptr);
            ~SvipWizard();

        public:
            Features features() const override;
            QString filter(Feature feature) const override;
            bool runWizard(Feature feature, IWizardContext *context) override;
        };

    }

}

#endif // CHORUSKIT_SVIPWIZARD_H
