#ifndef CHORUSKIT_ACEPWIZARD_H
#define CHORUSKIT_ACEPWIZARD_H

#include "iemgr/IWizardFactory.h"

#include "Dspx/QDspxModel.h"

namespace VarFmt::Internal {

    class AcepWizard : public IEMgr::IWizardFactory {
        Q_OBJECT
    public:
        explicit AcepWizard(QObject *parent = nullptr);
        ~AcepWizard();

        void reloadStrings();

    public:
        Features features() const override;
        QString filter(Feature feature) const override;
        bool runWizard(Feature feature, const QString &path, QWidget *parent) override;
    };

}

#endif // CHORUSKIT_ACEPWIZARD_H
