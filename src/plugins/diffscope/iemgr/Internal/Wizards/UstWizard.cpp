#include "UstWizard.h"

namespace IEMgr ::Internal {
    
    UstWizard::UstWizard(QObject *parent) : IImportWizard("iemgr_UstWizard", parent) {
        setDisplayName("UST file");
        setDescription("Utau sequence text file.");

        setCategory("SimpleFormats");
        setDisplayCategory(tr("Simple Formats"));
    }

    UstWizard::~UstWizard() {
    }

    QString UstWizard::filter() const {
        return QString("%1(%2)").arg(tr("Utau Sequence Texts"), "*.ust") + ";;" + IImportWizard::filter();
    }

    bool UstWizard::runWizard(WizardContext *context) {
        return false;
    }

} 