#include "SectionNotes.h"
#include "../Common/QUtauStd.h"

using namespace Utau;

SectionNotes::SectionNotes() {
    m_initTempo = DEFAULT_VALUE_TEMPO;
}

SectionNotes::SectionNotes(const QList<QLinkNote> &notes) {
    m_initTempo = DEFAULT_VALUE_TEMPO;
    setData(notes);
}

SectionNotes::SectionNotes(QList<QLinkNote> &&notes) {
    m_initTempo = DEFAULT_VALUE_TEMPO;
    swap(notes);
}

double SectionNotes::initTempo() const {
    return m_initTempo;
}

void SectionNotes::setInitTempo(double initTempo) {
    m_initTempo = initTempo;
}

QString SectionNotes::trackName() const {
    return m_trackName;
}

void SectionNotes::setTrackName(const QString &trackName) {
    m_trackName = trackName;
}

void SectionNotes::setData(QList<QLinkNote> &&notes) {
    swap(notes);
}

void SectionNotes::setData(const QList<QLinkNote> &notes) {
    clear();
    for (auto it = notes.begin(); it != notes.end(); ++it) {
        append(*it);
    }
}
