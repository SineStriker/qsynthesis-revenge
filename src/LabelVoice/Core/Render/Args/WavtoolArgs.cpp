#include "WavtoolArgs.h"

#include "Utau/Config/UtaConstants.h"
#include "Utau/Utils/UtaTranslator.h"

using namespace Utau;

WavtoolArgs::WavtoolArgs() {
    clear();
}

WavtoolArgs::~WavtoolArgs() {
}

void WavtoolArgs::clear() {
    m_inFile = "";
    m_outFile = "";

    m_startPoint = 0;
    m_voiceOverlap = 0;

    m_tempo = DEFAULT_VALUE_TEMPO;
    m_length = 480;
    m_correction = 0;

    m_rest = false;
}

QString WavtoolArgs::inFile() const {
    return m_inFile;
}

void WavtoolArgs::setInFile(const QString &inFile) {
    m_inFile = inFile;
}

QString WavtoolArgs::outFile() const {
    return m_outFile;
}

void WavtoolArgs::setOutFile(const QString &outFile) {
    m_outFile = outFile;
}

double WavtoolArgs::startPoint() const {
    return m_startPoint;
}

void WavtoolArgs::setStartPoint(double startPoint) {
    m_startPoint = startPoint;
}

double WavtoolArgs::voiceOverlap() const {
    return m_voiceOverlap;
}

void WavtoolArgs::setVoiceOverlap(double voiceOverlap) {
    m_voiceOverlap = voiceOverlap;
}

QList<QUstPoint> WavtoolArgs::envelope() const {
    return m_envelope;
}

void WavtoolArgs::setEnvelope(const QList<QUstPoint> &envelope) {
    m_envelope = envelope;
}

double WavtoolArgs::tempo() const {
    return m_tempo;
}

void WavtoolArgs::setTempo(double tempo) {
    m_tempo = tempo;
}

int WavtoolArgs::length() const {
    return m_length;
}

void WavtoolArgs::setLength(int length) {
    m_length = length;
}

double WavtoolArgs::correction() const {
    return m_correction;
}

void WavtoolArgs::setCorrection(double correction) {
    m_correction = correction;
}

bool WavtoolArgs::isRest() const {
    return m_rest;
}

void WavtoolArgs::setRest(bool rest) {
    m_rest = rest;
}

QString WavtoolArgs::outDuration() const {
    QString outDuration = QString::number(m_length) + "@" + QString::number(m_tempo);
    outDuration += ((m_correction >= 0) ? "+" : "") + QString::number(m_correction);

    return outDuration;
}

QStringList WavtoolArgs::envArguments() const {
    QStringList list;

    if (m_rest) {
        list << QString::number(0) << QString::number(0);
    } else {
        list << UtaTranslator::EnvelopeToStringList(m_envelope, m_voiceOverlap);
    }

    return list;
}

QStringList WavtoolArgs::fullArguments() const {
    QStringList list;

    list << m_outFile; // Arg 1: Input File (Normally a cache file generated by resampler)
    list << m_inFile;  // Arg 2: Output File

    list << QString::number(m_startPoint); // STP
    list << outDuration();                 // Fixed Duration

    list << envArguments();

    return list;
}

bool WavtoolArgs::operator==(const WavtoolArgs &another) const {
    if (&another == this) {
        return true;
    }
    if (m_rest != another.m_rest) {
        return false;
    }
    if (m_inFile != another.m_inFile) {
        return false;
    }
    if (m_outFile != another.m_outFile) {
        return false;
    }
    if (m_startPoint != another.m_startPoint) {
        return false;
    }
    if (m_length != another.m_length) {
        return false;
    }
    if (m_tempo != another.m_tempo) {
        return false;
    }
    if (m_correction != another.m_correction) {
        return false;
    }
    if (envArguments() != another.envArguments()) {
        return false;
    }
    return true;
}

bool WavtoolArgs::operator!=(const WavtoolArgs &another) const {
    return !((*this) == another);
}
