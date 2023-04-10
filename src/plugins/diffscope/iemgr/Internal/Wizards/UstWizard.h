#ifndef CHORUSKIT_USTWIZARD_H
#define CHORUSKIT_USTWIZARD_H

#include "iemgr/IWizardFactory.h"

namespace IEMgr {

    namespace Internal {

        class UstWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit UstWizard(QObject *parent = nullptr);
            ~UstWizard();

        public:
            QString filter() const override;
            Features features() const override;
            bool runWizard(Feature feature, const QString &path, Core::IWindow *windowHandle) override;
        };
    }
} // IEMgr

#endif // CHORUSKIT_USTWIZARD_H
