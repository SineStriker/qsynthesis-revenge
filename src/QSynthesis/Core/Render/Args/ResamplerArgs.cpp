#include "ResamplerArgs.h"
#include "../Utils/UtaPitchCurves.h"
#include "Utau/QUtaUtils.h"

#include <QFileInfo>

ResamplerArgs::ResamplerArgs() {
    clear();
}

ResamplerArgs::~ResamplerArgs() {
}

void ResamplerArgs::clear() {
    m_inFile = "";
    m_outFile = "";
    m_toneName = "C4";
    m_velocity = 100;

    m_flags = "";
    m_intensity = 100;
    m_modulation = 100;
    m_pitchCurves = {};

    m_sequence = -1;

    m_toBase64 = true;

    m_voiceTime = QDateTime();
    m_frqTime = QDateTime();
    m_mrqTime = QDateTime();
}

QString ResamplerArgs::inFile() const {
    return m_inFile;
}

void ResamplerArgs::setInFile(const QString &inFile) {
    m_inFile = inFile;

    m_voiceTime = QFileInfo(inFile).lastModified();
    m_frqTime = QFileInfo(QUtaUtils::toFrqName(inFile)).lastModified();
    m_mrqTime = QFileInfo(QUtaUtils::mrqName()).lastModified();
}

QString ResamplerArgs::outFile() const {
    return m_outFile;
}

void ResamplerArgs::setOutFile(const QString &outFile) {
    m_outFile = outFile;
}

QString ResamplerArgs::toneName() const {
    return m_toneName;
}

void ResamplerArgs::setToneName(const QString &toneName) {
    m_toneName = toneName;
}

void ResamplerArgs::setToneName(int noteNum) {
    m_toneName = QUtaUtils::ToneNumToToneName(noteNum);
}

double ResamplerArgs::velocity() const {
    return m_velocity;
}

void ResamplerArgs::setVelocity(double velocity) {
    m_velocity = velocity;
}

QString ResamplerArgs::flags() const {
    return m_flags;
}

void ResamplerArgs::setFlags(const QString &flags) {
    m_flags = flags;
}

double ResamplerArgs::intensity() const {
    return m_intensity;
}

void ResamplerArgs::setIntensity(double intensity) {
    m_intensity = intensity;
}

double ResamplerArgs::modulation() const {
    return m_modulation;
}

void ResamplerArgs::setModulation(double modulation) {
    m_modulation = modulation;
}

void ResamplerArgs::setPitchCurves(QList<int> pitches) {
    m_pitchCurves = pitches;
}

double ResamplerArgs::realLength() const {
    return m_realLength;
}

void ResamplerArgs::setRealLength(double realLength) {
    m_realLength = realLength;
}

bool ResamplerArgs::toBase64() const {
    return m_toBase64;
}

void ResamplerArgs::setToBase64(bool toBase64) {
    m_toBase64 = toBase64;
}

QStringList ResamplerArgs::paramsArguments() const {
    QStringList list;

    list << QString::number(m_intensity);
    list << QString::number(m_modulation);

    if (m_toBase64) {
        list << "!" + QString::number(m_tempo);
        list << UtaPitchCurves::encode_from_vector(m_pitchCurves);
    } else {
        // No using Base 64
        if (m_pitchCurves.isEmpty()) {
            list << QString::number(0) + "Q" + QString::number(m_tempo);
        } else {
            list << QString::number(m_pitchCurves.front()) + "Q" + QString::number(m_tempo);

            for (int i = 1; i < m_pitchCurves.size(); ++i) {
                list << QString::number(m_pitchCurves.at(i));
            }
        }
    }

    return list;
}

QStringList ResamplerArgs::fullArguments() const {
    QStringList list;

    list << m_inFile;   // Arg 1: Input file (Normally a sample in voicebank folder)
    list << m_outFile;  // Arg 2: Output file (Normally a cache file)
    list << m_toneName; // Arg 3: Tone Name

    list << QString::number(m_velocity); // Arg 4: Consonant Velocity
    list << m_flags;                     // Arg 5: Flags

    list << QString::number(m_genonSettings.Offset);   // Arg 6: Offset (Oto)
    list << QString::number(m_realLength);             // Arg 7: Corrected Duration
    list << QString::number(m_genonSettings.Constant); // Arg 8: Consonant (Oto)
    list << QString::number(m_genonSettings.Blank);    // Arg 9: Blank (Oto)

    list << paramsArguments();

    return list;
}

double ResamplerArgs::tempo() const {
    return m_tempo;
}

void ResamplerArgs::setTempo(double tempo) {
    m_tempo = tempo;
}

QList<int> ResamplerArgs::pitchCurves() const {
    return m_pitchCurves;
}

QOtoItem ResamplerArgs::genonSettings() const {
    return m_genonSettings;
}

void ResamplerArgs::setGenonSettings(const QOtoItem &genonSettings) {
    m_genonSettings = genonSettings;
}

QOtoItemBrief ResamplerArgs::correctGenon() const {
    return m_correctGenon;
}

void ResamplerArgs::setCorrectGenon(const QOtoItemBrief &correctGenon) {
    m_correctGenon = correctGenon;
}

int ResamplerArgs::sequence() const {
    return m_sequence;
}

void ResamplerArgs::setSequence(int sequence) {
    m_sequence = sequence;
}

bool ResamplerArgs::operator==(const ResamplerArgs &another) const {
    if (&another == this) {
        return true;
    }
    if (m_inFile != another.m_inFile) {
        return false;
    }
    if (m_voiceTime != another.m_voiceTime) {
        return false;
    }
    if (m_outFile != another.m_outFile) {
        return false;
    }
    if (m_toneName != another.m_toneName) {
        return false;
    }
    if (m_velocity != another.m_velocity) {
        return false;
    }
    if (m_flags != another.m_flags) {
        return false;
    }
    if (m_intensity != another.m_intensity) {
        return false;
    }
    if (m_modulation != another.m_modulation) {
        return false;
    }
    if (m_realLength != another.m_realLength) {
        return false;
    }
    if (m_genonSettings.Offset != another.m_genonSettings.Offset) {
        return false;
    }
    if (m_genonSettings.Constant != another.m_genonSettings.Constant) {
        return false;
    }
    if (m_genonSettings.Blank != another.m_genonSettings.Blank) {
        return false;
    }
    if (m_tempo != another.m_tempo) {
        return false;
    }
    if (m_pitchCurves != another.m_pitchCurves) {
        return false;
    }
    if (m_frqTime != another.m_frqTime) {
        return false;
    }
    if (m_mrqTime != another.m_mrqTime) {
        return false;
    }
    return true;
}

bool ResamplerArgs::operator!=(const ResamplerArgs &another) const {
    return !((*this) == another);
}

QDebug operator<<(QDebug debug, const ResamplerArgs &res) {
    debug.nospace() << "ResamplerArgs({" << Qt::endl
                    << res.m_correctGenon << Qt::endl
                    << "},{" << Qt::endl
                    << res.m_pitchCurves << Qt::endl
                    << "})";
    return debug;
}
