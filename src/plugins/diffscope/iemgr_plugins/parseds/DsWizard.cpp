#include "DsWizard.h"

#include <QMConsole.h>
#include <QMLinq.h>

#include <coreplugin/ICore.h>

#include "Utau/Utils/QUtaUtils.h"

namespace IEMgr ::Internal {

    DsWizard::DsWizard(QObject *parent) : IWizardFactory("iemgr.DsWizard", parent) {
        setDisplayName(tr("OpenVPI Ds file"));
        setDescription(tr("OpenVPI DiffSinger command line synthesizer format."));

        setCategory("score.OpenVPI");
        setDisplayCategory(tr("OpenVPI"));
    }

    DsWizard::~DsWizard() {
    }

    IWizardFactory::Features DsWizard::features() const {
        return {ImportProject | ExportProject};
    }

    QString DsWizard::filter(Feature feature) const {
        return QString("%1(%2)").arg(tr("OpenVPI Ds File"), "*.ds") + ";;" + IWizardFactory::filter(feature);
    }

    bool DsWizard::runWizard(Feature feature, const QString &path, const QVariantMap &args,
                             Core::IWindow *windowHandle) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                qDebug() << "import ds";
                break;
            }
            case IWizardFactory::ExportProject: {
                qDebug() << "export ds";
                break;
            }
            default:
                break;
        }
        return true;
    }

}