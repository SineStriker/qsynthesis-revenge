#include "QMidiConverter.h"

#include "QMidiFile.h"

#include "Utau/Config/UtaConstants.h"
#include "Utau/Utils/QUtaUtils.h"

QMidiConverter::QMidiConverter(QObject *parent) : ISVSConverter(parent) {
}

QMidiConverter::~QMidiConverter() {
}

bool QMidiConverter::load(const QString &filename, QSvipFile *out,
                          const QMap<QString, QVariant> &args) {
    QMidiFile midi;
    if (!midi.load(filename)) {
        return false;
    }

    int tracksCount = midi.tracks().size();
    if (tracksCount == 0) {
        return false;
    }

    return false;
}

bool QMidiConverter::save(const QString &filename, const QSvipFile &in,
                          const QMap<QString, QVariant> &args) {
    return false;
}
