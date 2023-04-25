#include "MusicTimeManager.h"
#include "MusicTimeManager_p.h"

#include <QDebug>

namespace ScriptMgr::Internal {

    MusicTimeManager::MusicTimeManager(tick_t tpqn): MusicTimeManager(*new MusicTimeManagerPrivate, tpqn) {
    }
    MusicTimeManager::~MusicTimeManager() {
    }
    MusicTimeManagerPrivate::MusicTimeManagerPrivate() {
    }
    MusicTimeManagerPrivate::~MusicTimeManagerPrivate() {
    }
    void MusicTimeManager::setTimeSignatures(const QList<QPair<measure_t, MusicTimeSignature>> &timeSignatureList) {
        Q_D(MusicTimeManager);
        bool isChanged = false;
        measure_t minPos = std::numeric_limits<measure_t>::max();
        for(const auto &pair: timeSignatureList) {
            if(pair.first < 0) {
                qDebug() << "Position of a time signature must be positive or zero, but read " << pair.first << " .";
                continue;
            }
            if(d->timeSignatureMap.contains(pair.first) && d->timeSignatureMap[pair.first] == pair.second) {
                //identical to current time signature
                continue;
            }
            if(!pair.second.isValid()) {
                qDebug() << "Invalid time signature '" << pair.second.toString() << "'.";
                continue;
            }
            isChanged = true;
            d->timeSignatureMap[pair.first] = pair.second;
            minPos = qMin(minPos, pair.first);
        }
        if(isChanged) {
            d->updateMeasureMap(minPos);
            emit timeSignatureChanged();
            emit changed();
        }
    }
    void MusicTimeManagerPrivate::updateMeasureMap(measure_t barFrom) {
        Q_Q(MusicTimeManager);
        auto iter = timeSignatureMap.lowerBound(barFrom);
        if(iter == timeSignatureMap.begin()) iter++;
        for(auto it = measureMap.upperBound(revMeasureMap[(iter - 1).key()]); it != measureMap.end();) {
            revMeasureMap.remove(it.value());
            it = measureMap.erase(it);
        }
        for(; iter != timeSignatureMap.end(); iter++) {
            auto prevMeasure = (iter - 1).key();
            auto prevTimeSig = (iter - 1).value();
            auto prevTickPos = revMeasureMap[prevMeasure];
            auto currentTickPos = prevTickPos + (iter.key() - prevMeasure) * prevTimeSig.ticksPerBar(tpqn);
            measureMap[currentTickPos] = iter.key();
            revMeasureMap[iter.key()] = currentTickPos;
        }
    }
    tick_t MusicTimeManagerPrivate::findNearestTickWithTimeSignature(tick_t tick) const {
        if(tick < 0) return 0;
        auto it = measureMap.upperBound(tick);
        if(it == measureMap.end()) {
            return measureMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }
    QList<QPair<measure_t, MusicTimeSignature>> MusicTimeManager::timeSignatures() const {
        Q_D(const MusicTimeManager);
        QList<QPair<measure_t, MusicTimeSignature>> list;
        for(auto key: d->timeSignatureMap.keys()) {
            list.append(qMakePair(key, d->timeSignatureMap[key]));
        }
        return list;
    }
    void MusicTimeManager::removeTimeSignatures(const QList<measure_t> &barPosList) {
        Q_D(MusicTimeManager);
        bool isChanged = false;
        measure_t minPos = std::numeric_limits<measure_t>::max();
        for(auto barPos: barPosList) {
            if(barPos <= 0) {
                qDebug() << "Position of a time signature must be positive, but read " << barPos << " .";
                continue;
            }
            if(!d->timeSignatureMap.contains(barPos)) {
                qDebug() << "Bar " << barPos << " does not have time signature.";
                continue;
            }

            isChanged = true;
            d->timeSignatureMap.remove(barPos);
            minPos = qMin(minPos, barPos);
        }
        if(isChanged) {
            d->updateMeasureMap(minPos);
            emit timeSignatureChanged();
            emit changed();
        }
    }
    void MusicTimeManager::setTimeSignature(measure_t barPos, MusicTimeSignature timeSignature) {
        setTimeSignatures({{barPos, timeSignature}});
    }
    MusicTimeSignature MusicTimeManager::timeSignature(measure_t barPos) const {
        Q_D(const MusicTimeManager);
        return d->timeSignatureMap[nearestTimeSignaturePosition(barPos)];
    }
    measure_t MusicTimeManager::nearestTimeSignaturePosition(measure_t barPos) const {
        Q_D(const MusicTimeManager);
        if(barPos < 0) {
            qDebug() << "Position of a time signature must be positive or zero.";
            return 0;
        }
        auto it = d->timeSignatureMap.upperBound(barPos);
        if(it == d->timeSignatureMap.end()) {
            return d->timeSignatureMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }
    void MusicTimeManager::removeTimeSignature(measure_t barPos) {
        removeTimeSignatures({barPos});
    }
    void MusicTimeManager::setTempos(const QList<QPair<tick_t, tempo_t>> &tempoList) {
        Q_D(MusicTimeManager);
        bool isChanged = false;
        tick_t minPos = std::numeric_limits<tick_t>::max();
        for(const auto &pair: tempoList) {
            if(pair.first < 0) {
                qDebug() << "Position of a tempo must be positive or zero, but read " << pair.first << " .";
                continue;
            }
            if(d->msecSumMap.contains(pair.first) && d->msecSumMap[pair.first] == pair.second) {
                //identical to current time signature
                continue;
            }
            if(pair.second < 0) {
                qDebug() << "Invalid tempo '" << pair.second << "'.";
                continue;
            }
            isChanged = true;
            d->tempoMap[pair.first] = pair.second;
            minPos = qMin(minPos, pair.first);
        }
        if(isChanged) {
            d->updateMsecSumMap(minPos);
            emit tempoChanged();
            emit changed();
        }
    }
    void MusicTimeManagerPrivate::updateMsecSumMap(tick_t tickFrom) {
        Q_Q(MusicTimeManager);
        auto iter = tempoMap.lowerBound(tickFrom);
        if(iter == tempoMap.begin()) iter++;
        for(auto it = msecSumMap.upperBound((iter - 1).key()); it != msecSumMap.end();) {
            revMsecSumMap.remove(it.value());
            it = msecSumMap.erase(it);
        }
        for(; iter != tempoMap.end(); iter++) {
            auto prevTick = (iter - 1).key();
            auto prevTempo = (iter - 1).value();
            auto prevMsec = msecSumMap[prevTick];
            auto currentMsec = prevMsec + (iter.key() - prevTick) * (60.0 * 1000.0) / (tpqn * prevTempo);
            msecSumMap[iter.key()] = currentMsec;
            revMsecSumMap[currentMsec] = iter.key();
        }
    }
    msec_t MusicTimeManagerPrivate::findNearestMsecWithTempo(msec_t msec) const {
        if(msec < 0) return 0;
        auto it = revMsecSumMap.upperBound(msec);
        if(it == revMsecSumMap.end()) {
            return revMsecSumMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }
    QList<QPair<tick_t, tempo_t>> MusicTimeManager::tempos() const {
        Q_D(const MusicTimeManager);
        QList<QPair<tick_t, tempo_t>> list;
        for(auto key: d->tempoMap.keys()) {
            list.append(qMakePair(key, d->tempoMap[key]));
        }
        return list;
    }
    void MusicTimeManager::removeTempos(const QList<tick_t> &tickPosList) {
        Q_D(MusicTimeManager);
        bool isChanged = false;
        tick_t minPos = std::numeric_limits<tick_t>::max();
        for(const auto tickPos: tickPosList) {
            if(tickPos <= 0) {
                qDebug() << "Position of a tempo must be positive, but read " << tickPos << " .";
                continue;
            }
            if(!d->msecSumMap.contains(tickPos)) {
                qDebug() << "Tick " << tickPos << " does not have tempo.";
                continue;
            }
            isChanged = true;
            d->tempoMap.remove(tickPos);
            minPos = qMin(minPos, tickPos);
        }
        if(isChanged) {
            d->updateMsecSumMap(minPos);
            emit tempoChanged();
            emit changed();
        }
    }
    void MusicTimeManager::setTempo(tick_t tickPos, tempo_t tempo) {
        setTempos({{tickPos, tempo}});
    }
    tempo_t MusicTimeManager::tempo(tick_t tickPos) const {
        Q_D(const MusicTimeManager);
        return d->tempoMap[nearestTempoPosition(tickPos)];
    }
    tick_t MusicTimeManager::nearestTempoPosition(tick_t tickPos) const {
        Q_D(const MusicTimeManager);
        if(tickPos < 0) {
            qDebug() << "Position of a tempo must be positive or zero.";
            return 0;
        }
        auto it = d->tempoMap.upperBound(tickPos);
        if(it == d->tempoMap.end()) {
            return d->tempoMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }
    void MusicTimeManager::removeTempo(tick_t tickPos) {
        removeTempos({tickPos});
    }
    MusicTime MusicTimeManager::create() {
        return MusicTime(this);
    }
    MusicTime MusicTimeManager::create(measure_t measure, beat_t beat, tick_t tick) {
        return MusicTime(this, measure, beat, tick);
    }
    MusicTime MusicTimeManager::create(const QString &str) {
        return MusicTime(this, str);
    }
    MusicTime MusicTimeManager::create(msec_t msec) {
        return MusicTime(this, msec);
    }
    MusicTimeManager::MusicTimeManager(MusicTimeManagerPrivate &d, tick_t tpqn): d_ptr(&d) {
        d.q_ptr = this;
        d.tpqn = tpqn;
        d.timeSignatureMap = {{0, {4, 4}}};
        d.tempoMap = {{0, 120}};
        d.measureMap = {{0, 0}};
        d.revMeasureMap = {{0, 0}};
        d.msecSumMap = {{0, 0}};
        d.revMsecSumMap = {{0, 0}};
    }
    void MusicTimeManager::thisIsATest() {
        Q_D(MusicTimeManager);
        setTimeSignature(1, {3, 8});
        setTimeSignature(3, {5,4});
        qDebug() << "====Test====";
        qDebug() << d->timeSignatureMap.keys();
        qDebug() << d->timeSignatureMap.values();
        qDebug() << d->measureMap.keys();
        qDebug() << d->measureMap.values();
        setTimeSignature(2, {5, 4});
        qDebug() << "====Test====";
        qDebug() << d->timeSignatureMap.keys();
        qDebug() << d->timeSignatureMap.values();
        qDebug() << d->measureMap.keys();
        qDebug() << d->measureMap.values();
        removeTimeSignature(1);
        qDebug() << "====Test====";
        qDebug() << d->timeSignatureMap.keys();
        qDebug() << d->timeSignatureMap.values();
        qDebug() << d->measureMap.keys();
        qDebug() << d->measureMap.values();
        setTimeSignature(0, {3, 4});
        qDebug() << "====Test====";
        qDebug() << d->timeSignatureMap.keys();
        qDebug() << d->timeSignatureMap.values();
        qDebug() << d->measureMap.keys();
        qDebug() << d->measureMap.values();
        setTempo(2400, 90);
        setTempo(1200, 80);
        setTempo(1920, 60);
        qDebug() << "====Test2====";
        qDebug() << d->tempoMap.keys();
        qDebug() << d->tempoMap.values();
        qDebug() << d->msecSumMap.keys();
        qDebug() << d->msecSumMap.values();
        auto mt = create();
        mt.m_tick = 5700;
        qDebug() << mt.toString();
        mt.m_tick = 4000;
        qDebug() << mt.toString();
        mt.fromString("3:3:160");
        qDebug() << mt.m_tick;
        mt.setMbt(3, 0, 420);
        qDebug() << mt.m_tick;
        qDebug() << mt.msec();
        mt.fromMsec(2875);
        qDebug() << mt.toString();
        mt.fromMsec(3000);
        qDebug() << mt.toString();
    }

} // Internal