#include "AcepWizard.h"

#include <QApplication>

#include <QMDecoratorV2.h>

namespace VarFmt ::Internal {

    using namespace IEMgr;

    AcepWizard::AcepWizard(QObject *parent) : IWizardFactory("iemgr.AcepWizard", parent) {
        setCategory("score.TimeDomAIn");
        qIDec->installLocale(this, _LOC(AcepWizard, this));
    }

    AcepWizard::~AcepWizard() {
    }

    void AcepWizard::reloadStrings() {
        setDisplayName(tr("ACEP File"));
        setDescription(tr("ACE Studio project file."));
        setDisplayCategory(QApplication::translate("IEMgr::WizardCategory", "TimeDomAIn"));
    }

    IWizardFactory::Features AcepWizard::features() const {
        return {ImportProject | ExportProject};
    }

    QString AcepWizard::filter(Feature feature) const {
        return QString("%1(%2)").arg(tr("ACE Studio Files"), "*.acep") + ";;" + IWizardFactory::filter(feature);
    }

    bool AcepWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                qDebug() << "import acep";
                break;
            }
            case IWizardFactory::ExportProject: {
                qDebug() << "export acep";
                break;
            }
            default:
                break;
        }
        return true;
    }

}