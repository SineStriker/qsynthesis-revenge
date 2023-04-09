#include "MidiWizard.h"

namespace IEMgr ::Internal {

    MidiWizard::MidiWizard(QObject *parent) : IImportWizard("iemgr_MidiWizard", parent) {
        setDisplayName("Midi file");
        setDescription("A communications protocol for media interfaces.");

        setCategory("SimpleFormats");
        setDisplayCategory(tr("Simple Formats"));
    }

    MidiWizard::~MidiWizard() {
    }

    QString MidiWizard::filter() const {
        return QString("%1(%2)").arg(tr("Standard Midi Files"), "*.mid *.midi") + ";;" + IImportWizard::filter();
    }

    bool MidiWizard::runWizard(WizardContext *context) {
        return false;
    }

}