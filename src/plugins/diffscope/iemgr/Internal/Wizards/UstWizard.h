#ifndef USTWIZARD_H
#define USTWIZARD_H

#include "iemgr/IWizardFactory.h"

#include "Dspx/QDspxModel.h"

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

        private:
            bool load(const QString &filename, QDspxModel *out, QWidget *parent = nullptr);
            bool save(const QString &filename, const QDspxModel &in, QWidget *parent = nullptr);
        };
    }
}

#endif // USTWIZARD_H
