#include "MusicTimeManager.h"
#include "MusicTimeManager_p.h"

namespace ScriptMgr::Internal {

    MusicTimeManager::MusicTimeManager(): MusicTimeManager(*new MusicTimeManagerPrivate) {
    }
    void MusicTimeManager::setTpqn(int tpqn) {
    }
    int MusicTimeManager::tpqn() const {
        return 0;
    }
    void MusicTimeManager::setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList) {
    }
    QList<QPair<int, MusicTimeSignature>> MusicTimeManager::timeSignatures() const {
        return QList<QPair<int, MusicTimeSignature>>();
    }
    void MusicTimeManager::setTimeSignature(int barPos, MusicTimeSignature timeSignature) {
    }
    int MusicTimeManager::timeSignature(int barPos) const {
        return 0;
    }
    int MusicTimeManager::nearestTimeSignaturePosition(int barPos) const {
        return 0;
    }
    void MusicTimeManager::removeTimeSiganture(int barPos) {
    }
    void MusicTimeManager::setTempos(const QList<QPair<int, double>> &tempoList) {
    }
    QList<QPair<int, double>> MusicTimeManager::tempos() const {
        return QList<QPair<int, double>>();
    }
    void MusicTimeManager::setTempo(int tickPos, double tempo) {
    }
    double MusicTimeManager::tempo(int tickPos) const {
        return 0;
    }
    int MusicTimeManager::nearestTempoPosition(int tickPos) const {
        return 0;
    }
    void MusicTimeManager::removeTempo(int tickPos) {
    }
    MusicTime MusicTimeManager::create() {
        return MusicTime(this);
    }
    MusicTime MusicTimeManager::create(int measure, int beat, int tick) {
        return MusicTime(this, measure, beat, tick);
    }
    MusicTime MusicTimeManager::create(const QString &str) {
        return MusicTime(this, str);
    }
    MusicTimeManager::MusicTimeManager(MusicTimeManagerPrivate &d): d_ptr(&d) {
        d.q_ptr = this;
        d.tpqn = 480;
        d.timeSignatureMap = {{0, {4, 4}}};
        d.tempoMap = {{0, 120}};
        d.measureMap = {{0, 0}};
        d.msecSumMap = {{0, 0}};
    }

} // Internal