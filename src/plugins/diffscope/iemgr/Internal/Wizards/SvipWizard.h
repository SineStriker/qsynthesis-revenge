#ifndef CHORUSKIT_SVIPWIZARD_H
#define CHORUSKIT_SVIPWIZARD_H

#include "Dspx/QDspxModel.h"
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
            bool runWizard(Feature feature, const QString &path, QWidget *parent) override;

        private:
            bool load(const QString &filename, QDspxModel *out, QWidget *parent = nullptr);
            bool save(const QString &filename, const QDspxModel &in, QWidget *parent = nullptr);
            static double toLinearVolume(const double &gain);
            static double toDecibelGain(const double &volume);
            static double log10(const double &x);
        };

    }

}

#endif // CHORUSKIT_SVIPWIZARD_H
