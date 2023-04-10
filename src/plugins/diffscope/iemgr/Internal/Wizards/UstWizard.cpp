#include "UstWizard.h"

namespace IEMgr ::Internal {

    UstWizard::UstWizard(QObject *parent) : IWizardFactory("iemgr.UstWizard", parent) {
        setDisplayName("UST file");
        setDescription("Utau sequence text file.");

        setCategory("import.SimpleFormats");
        setDisplayCategory(tr("Simple Formats"));
    }

    UstWizard::~UstWizard() {
    }

    QString UstWizard::filter() const {
        return QString("%1(%2)").arg(tr("Utau Sequence Texts"), "*.ust") + ";;" + IWizardFactory::filter();
    }

    IWizardFactory::Features UstWizard::features() const {
        return {Import | Export};
    }

    bool UstWizard::runWizard(Feature feature, const QString &path, Core::IWindow *windowHandle) {
        return false;
    }

}