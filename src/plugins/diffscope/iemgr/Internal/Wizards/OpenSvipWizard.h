#ifndef CHORUSKIT_OPENSVIPWIZARD_H
#define CHORUSKIT_OPENSVIPWIZARD_H

#include "iemgr/IWizardFactory.h"
#include "Dspx/QDspxModel.h"

namespace IEMgr {

    namespace Internal {

        class OpenSvipWizard : public IWizardFactory {
            Q_OBJECT
        public:
            explicit OpenSvipWizard(QObject *parent = nullptr);
            ~OpenSvipWizard();

            void reloadStrings();

        public:
            Features features() const override;
            QString filter(Feature feature) const override;
            bool runWizard(Feature feature, const QString &path, QWidget *parent) override;

        private:
            bool load(const QString &filename, QDspxModel *out, QWidget *parent = nullptr);
            bool save(const QString &filename, const QDspxModel &in, QWidget *parent = nullptr);
            bool loadProjectFile(const QString &filename, QJsonObject *out);
        };

    }

}


#endif // CHORUSKIT_OPENSVIPWIZARD_H
