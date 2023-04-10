#ifndef CHORUSKIT_MIDIWIZARD_H
#define CHORUSKIT_MIDIWIZARD_H

#include "iemgr/IWizardFactory.h"

#include "Dspx/QDspxModel.h"

namespace IEMgr {

    namespace Internal {

        class MidiWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit MidiWizard(QObject *parent = nullptr);
            ~MidiWizard();

        public:
            Features features() const override;
            QString filter(Feature feature) const override;
            bool runWizard(Feature feature, const QString &path, Core::IWindow *windowHandle) override;

        private:
            bool load(const QString &filename, QDspxModel *out, QWidget *parent = nullptr);
            bool save(const QString &filename, const QDspxModel &in, QWidget *parent = nullptr) ;
        };

    }

} // IEMgr

#endif // CHORUSKIT_MIDIWIZARD_H
