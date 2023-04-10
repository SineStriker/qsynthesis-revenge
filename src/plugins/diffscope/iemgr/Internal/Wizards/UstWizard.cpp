#include "UstWizard.h"

namespace IEMgr ::Internal {

    UstWizard::UstWizard(QObject *parent) : IWizardFactory("iemgr.UstWizard", parent) {
        setDisplayName("UST file");
        setDescription("Utau sequence text file.");

        setCategory("score.SimpleScore");
        setDisplayCategory(tr("Simple Score"));
    }

    UstWizard::~UstWizard() {
    }

    QString UstWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("Utau Sequence Texts"), "*.ust") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features UstWizard::features() const {
        return {Import | Export};
    }

    bool UstWizard::runWizard(Feature feature, const QString &path, Core::IWindow *windowHandle) {
        return false;
    }

}