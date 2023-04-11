#include "OpenSvipWizard.h"

namespace IEMgr ::Internal {

    OpenSvipWizard::OpenSvipWizard(QObject *parent) : IWizardFactory("iemgr.OpenSvipWizard", parent) {
        setDisplayName(tr("OpenSVIP model"));
        setDescription(tr("OpenSVIP converter intermediate model."));

        setCategory("score.OpenVPI");
        setDisplayCategory(tr("OpenVPI"));
    }

    OpenSvipWizard::~OpenSvipWizard() {
    }

    QString OpenSvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("OpenSVIP Json Files"), "*.json") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features OpenSvipWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool OpenSvipWizard::runWizard(Feature feature, IWizardContext *context) {
        switch (feature) {
            case IWizardFactory::ImportProject:
                break;
            case IWizardFactory::ExportProject:
                break;
        }
        return false;
    }

}