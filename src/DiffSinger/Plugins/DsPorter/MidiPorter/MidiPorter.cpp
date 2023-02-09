#include "MidiPorter.h"

#include "QMidiFile.h"

MidiPorter::MidiPorter(QObject *parent) : IDsPorter(parent) {
}

MidiPorter::~MidiPorter() {
}

QString MidiPorter::filterString() const {
    return tr("Standard MIDI Files(*.mid)");
};

bool MidiPorter::load(const QString &filename, QDspxModel *out,
                      const QMap<QString, QVariant> &args) {
    return false;
}

bool MidiPorter::save(const QString &filename, const QDspxModel &in,
                      const QMap<QString, QVariant> &args) {
    return false;
}
