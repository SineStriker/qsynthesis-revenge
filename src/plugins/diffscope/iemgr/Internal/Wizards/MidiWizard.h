#ifndef CHORUSKIT_MIDIWIZARD_H
#define CHORUSKIT_MIDIWIZARD_H

#include "iemgr/IImportWizard.h"

namespace IEMgr {

    namespace Internal {

        class MidiWizard : public IImportWizard {
            Q_OBJECT
        public:
            explicit MidiWizard(QObject *parent = nullptr);
            ~MidiWizard();

        public:
            QString filter() const override;
            bool runWizard(Core::IWindow *windowHandle) override;
        };

    }

} // IEMgr

#endif // CHORUSKIT_MIDIWIZARD_H
