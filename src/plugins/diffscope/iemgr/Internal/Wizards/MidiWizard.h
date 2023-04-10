#ifndef CHORUSKIT_MIDIWIZARD_H
#define CHORUSKIT_MIDIWIZARD_H

#include "iemgr/IWizardFactory.h"

namespace IEMgr {

    namespace Internal {

        class MidiWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit MidiWizard(QObject *parent = nullptr);
            ~MidiWizard();

        public:
            QString filter() const override;
            Features features() const override;
            bool runWizard(Feature feature, const QString &path, Core::IWindow *windowHandle) override;
        };

    }

} // IEMgr

#endif // CHORUSKIT_MIDIWIZARD_H
