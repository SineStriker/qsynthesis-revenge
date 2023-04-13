#include "Svip3Wizard.h"

#include <QMConsole.h>
#include <QMLinq.h>

#include <coreplugin/ICore.h>

namespace IEMgr ::Internal {

    Svip3Wizard::Svip3Wizard(QObject *parent) : IWizardFactory("iemgr.Svip3Wizard", parent) {
        setDisplayName(tr("SVIP3 file"));
        setDescription(tr("XStudio 3.0 project file."));

        setCategory("score.XiaoIce");
        setDisplayCategory(tr("XiaoIce"));
        
        qDebug() << "bug fixed";
    }

    Svip3Wizard::~Svip3Wizard() {
    }

    IWizardFactory::Features Svip3Wizard::features() const {
        return {ImportProject | ExportProject};
    }

    QString Svip3Wizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("XStudio3 Svip Files"), "*.svip3") + ";;" + IWizardFactory::filter(feature);
    }

    bool Svip3Wizard::runWizard(Feature feature, IWizardContext *context) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                qDebug() << "import svip3";
                break;
            }
            case IWizardFactory::ExportProject: {
                qDebug() << "export svip3";
                break;
            }
            default:
                break;
        }
        return true;
    }

}