#ifndef DSWIZARD_H
#define DSWIZARD_H

#include "iemgr/IWizardFactory.h"

#include "Dspx/QDspxModel.h"

namespace VarFmt::Internal {

    class DsWizard : public IEMgr::IWizardFactory {
        Q_OBJECT
    public:
        explicit DsWizard(QObject *parent = nullptr);
        ~DsWizard();

    public:
        Features features() const override;
        QString filter(Feature feature) const override;
        bool runWizard(Feature feature, const QString &path, QWidget *parent) override;
    };

}


#endif // CHORUSKIT_MIDIWIZARD_H
