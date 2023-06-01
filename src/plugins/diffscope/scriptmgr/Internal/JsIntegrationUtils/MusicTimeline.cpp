#include "MusicTimeline.h"
#include "MusicTimeline_p.h"

#include <QDebug>

namespace ScriptMgr::Internal {

    MusicTimeline::MusicTimeline(): MusicTimeline(*new MusicTimelinePrivate) {
    }

    MusicTimeline::~MusicTimeline() {
    }

    MusicTimelinePrivate::MusicTimelinePrivate() {
    }

    MusicTimelinePrivate::~MusicTimelinePrivate() {
    }

    void MusicTimelinePrivate::init() {
        tpqn = 480;
        timeSignatureMap = {{0, {4, 4}}};
        tempoMap = {{0, 120}};
        measureMap = {{0, 0}};
        revMeasureMap = {{0, 0}};
        msecSumMap = {{0, 0}};
        revMsecSumMap = {{0, 0}};
    }

    void MusicTimelinePrivate::updateMeasureMap(int barFrom) {
        Q_Q(MusicTimeline);
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

    void MusicTimelinePrivate::updateMsecSumMap(int tickFrom) {
        Q_Q(MusicTimeline);
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

    int MusicTimelinePrivate::findNearestTickWithTimeSignature(int tick) const {
        if(tick < 0) return 0;
        auto it = measureMap.upperBound(tick);
        if(it == measureMap.end()) {
            return measureMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }

    double MusicTimelinePrivate::findNearestMsecWithTempo(double msec) const {
        if(msec < 0) return 0;
        auto it = revMsecSumMap.upperBound(msec);
        if(it == revMsecSumMap.end()) {
            return revMsecSumMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }

    void MusicTimelinePrivate::_q_timeSignatureChanged() {
        for(auto *t: mbtCachedMusicTimes) {
            t->m_mbtDirty = true;
        }
        mbtCachedMusicTimes.clear();
    }

    void MusicTimelinePrivate::_q_tempoChanged() {
        for(auto *t: msecCachedMusicTimes) {
            t->m_cache.msec = -1;
        }
        msecCachedMusicTimes.clear();
    }



    void MusicTimeline::setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList) {
        Q_D(MusicTimeline);
        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for(const auto &pair: timeSignatureList) {
            if(pair.first < 0) {
                qWarning() << "Position of a time signature must be positive or zero, but read " << pair.first << " .";
                continue;
            }
            if(d->timeSignatureMap.contains(pair.first) && d->timeSignatureMap[pair.first] == pair.second) {
                //identical to current time signature
                continue;
            }
            if(!pair.second.isValid()) {
                qWarning() << "Invalid time signature '" << pair.second.toString() << "'.";
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

    QList<QPair<int, MusicTimeSignature>> MusicTimeline::timeSignatures() const {
        Q_D(const MusicTimeline);
        QList<QPair<int, MusicTimeSignature>> list;
        for(auto key: d->timeSignatureMap.keys()) {
            list.append(qMakePair(key, d->timeSignatureMap[key]));
        }
        return list;
    }

    void MusicTimeline::removeTimeSignatures(const QList<int> &barPosList) {
        Q_D(MusicTimeline);
        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for(auto barPos: barPosList) {
            if(barPos <= 0) {
                qWarning() << "Position of a time signature must be positive, but read " << barPos << " .";
                continue;
            }
            if(!d->timeSignatureMap.contains(barPos)) {
                qWarning() << "Bar " << barPos << " does not have time signature.";
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

    void MusicTimeline::setTimeSignature(int barPos, MusicTimeSignature timeSignature) {
        setTimeSignatures({{barPos, timeSignature}});
    }

    MusicTimeSignature MusicTimeline::timeSignature(int barPos) const {
        Q_D(const MusicTimeline);
        return d->timeSignatureMap[nearestTimeSignaturePosition(barPos)];
    }

    int MusicTimeline::nearestTimeSignaturePosition(int barPos) const {
        Q_D(const MusicTimeline);
        if(barPos < 0) {
            qWarning() << "Position of a time signature must be positive or zero.";
            return 0;
        }
        auto it = d->timeSignatureMap.upperBound(barPos);
        if(it == d->timeSignatureMap.end()) {
            return d->timeSignatureMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }

    void MusicTimeline::removeTimeSignature(int barPos) {
        removeTimeSignatures({barPos});
    }

    void MusicTimeline::setTempos(const QList<QPair<int, double>> &tempoList) {
        Q_D(MusicTimeline);
        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for(const auto &pair: tempoList) {
            if(pair.first < 0) {
                qWarning() << "Position of a tempo must be positive or zero, but read " << pair.first << " .";
                continue;
            }
            if(d->msecSumMap.contains(pair.first) && d->msecSumMap[pair.first] == pair.second) {
                //identical to current time signature
                continue;
            }
            if(pair.second <= 0) {
                qWarning() << "Invalid tempo '" << pair.second << "'.";
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

    QList<QPair<int, double>> MusicTimeline::tempos() const {
        Q_D(const MusicTimeline);
        QList<QPair<int, double>> list;
        for(auto key: d->tempoMap.keys()) {
            list.append(qMakePair(key, d->tempoMap[key]));
        }
        return list;
    }

    void MusicTimeline::removeTempos(const QList<int> &tickPosList) {
        Q_D(MusicTimeline);
        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for(const auto tickPos: tickPosList) {
            if(tickPos <= 0) {
                qWarning() << "Position of a tempo must be positive, but read " << tickPos << " .";
                continue;
            }
            if(!d->msecSumMap.contains(tickPos)) {
                qWarning() << "Tick " << tickPos << " does not have tempo.";
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

    void MusicTimeline::setTempo(int tickPos, double tempo) {
        setTempos({{tickPos, tempo}});
    }

    double MusicTimeline::tempo(int tickPos) const {
        Q_D(const MusicTimeline);
        return d->tempoMap[nearestTempoPosition(tickPos)];
    }

    int MusicTimeline::nearestTempoPosition(int tickPos) const {
        Q_D(const MusicTimeline);
        if(tickPos < 0) {
            qWarning() << "Position of a tempo must be positive or zero.";
            return 0;
        }
        auto it = d->tempoMap.upperBound(tickPos);
        if(it == d->tempoMap.end()) {
            return d->tempoMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }

    void MusicTimeline::removeTempo(int tickPos) {
        removeTempos({tickPos});
    }

    MusicTime MusicTimeline::create() {
        return MusicTime(this);
    }

    MusicTime MusicTimeline::create(int measure, int beat, int tick) {
        return MusicTime(this, measure, beat, tick);
    }

    MusicTime MusicTimeline::create(const QString &str) {
        return MusicTime(this, str);
    }

    MusicTime MusicTimeline::create(double msec) {
        return MusicTime(this, msec);
    }

    MusicTimeline::MusicTimeline(MusicTimelinePrivate &d): d_ptr(&d) {
        d.q_ptr = this;
        d.init();
        connect(this, &MusicTimeline::timeSignatureChanged, &d, &MusicTimelinePrivate::_q_timeSignatureChanged);
        connect(this, &MusicTimeline::tempoChanged, &d, &MusicTimelinePrivate::_q_tempoChanged);
    }

    void MusicTimeline::thisIsATest() {
        Q_D(MusicTimeline);
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
        mt = create(0, 0, 5700);
        qDebug() << mt.toString();
        mt = create(0, 0, 4000);
        qDebug() << mt.toString();
        mt = create("3:3:160");
        qDebug() << mt.totalTick();
        mt = create(3, 0, 420);
        qDebug() << mt.totalTick();
        qDebug() << mt.msec();
        mt = create(2875);
        qDebug() << mt.toString();
        mt = create(3000);
        qDebug() << mt.toString();
        qDebug() << mt.msec();
        removeTempos({2400, 1920, 1200});
        qDebug() << mt.msec();
    }

} // Internal