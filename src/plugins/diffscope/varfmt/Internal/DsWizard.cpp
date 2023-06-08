#include "DsWizard.h"

#include <QApplication>

#include <QMConsole.h>
#include <QMDecoratorV2.h>
#include <QMLinq.h>

#include <coreplugin/ICore.h>

#include "Utau/Utils/QUtaUtils.h"

namespace VarFmt ::Internal {

    using namespace IEMgr;

    DsWizard::DsWizard(QObject *parent) : IWizardFactory("iemgr.DsWizard", parent) {
        setCategory("score.OpenVPI");
        setDisplayName([]() { return tr("OpenVPI Ds file"); });
        setDescription([]() { return tr("OpenVPI DiffSinger command line synthesizer format."); });
        setDisplayCategory([]() { return QApplication::translate("IEMgr::WizardCategory", "OpenVPI"); });
    }

    DsWizard::~DsWizard() {
    }

    IWizardFactory::Features DsWizard::features() const {
        return {ImportProject | ExportProject};
    }

    QString DsWizard::filter(Feature feature) const {
        return QString("%1(%2)").arg(tr("OpenVPI Ds Files"), "*.ds") + ";;" + IWizardFactory::filter(feature);
    }

    bool DsWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
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