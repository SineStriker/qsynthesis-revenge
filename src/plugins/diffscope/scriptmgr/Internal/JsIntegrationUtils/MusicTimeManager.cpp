#include "MusicTimeManager.h"
#include "MusicTimeManager_p.h"

#include <QDebug>

namespace ScriptMgr::Internal {

    MusicTimeManager::MusicTimeManager(int tpqn): MusicTimeManager(*new MusicTimeManagerPrivate, tpqn) {
    }
    MusicTimeManager::~MusicTimeManager() {
    }
    MusicTimeManagerPrivate::MusicTimeManagerPrivate() {
    }
    MusicTimeManagerPrivate::~MusicTimeManagerPrivate() {
    }
    void MusicTimeManager::setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList) {
        Q_D(MusicTimeManager);
        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
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
            emit tempoChanged();
            emit changed();
        }
    }
    void MusicTimeManagerPrivate::updateMeasureMap(int barFrom) {
        Q_Q(MusicTimeManager);
        auto iter = timeSignatureMap.lowerBound(barFrom);
        if(iter == timeSignatureMap.begin()) iter++;
        for(auto it = measureMap.upperBound(measureMap.key((iter - 1).key())); it != measureMap.end(); it = measureMap.erase(it));
        for(; iter != timeSignatureMap.end(); iter++) {
            auto prevMeasure = (iter - 1).key();
            auto prevTimeSig = (iter - 1).value();
            auto prevTickPos = measureMap.key(prevMeasure);
            auto currentTickPos = prevTickPos + (iter.key() - prevMeasure) * prevTimeSig.ticksPerBar(tpqn);
            measureMap[currentTickPos] = iter.key();
        }
    }
    QList<QPair<int, MusicTimeSignature>> MusicTimeManager::timeSignatures() const {
        Q_D(const MusicTimeManager);
        QList<QPair<int, MusicTimeSignature>> list;
        for(auto key: d->timeSignatureMap.keys()) {
            list.append(qMakePair(key, d->timeSignatureMap[key]));
        }
        return list;
    }
    void MusicTimeManager::removeTimeSignatures(const QList<int> &barPosList) {
        Q_D(MusicTimeManager);
        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
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
            emit tempoChanged();
            emit changed();
        }
    }
    void MusicTimeManager::setTimeSignature(int barPos, MusicTimeSignature timeSignature) {
        setTimeSignatures({{barPos, timeSignature}});
    }
    MusicTimeSignature MusicTimeManager::timeSignature(int barPos) const {
        Q_D(const MusicTimeManager);
        return d->timeSignatureMap[nearestTimeSignaturePosition(barPos)];
    }
    int MusicTimeManager::nearestTimeSignaturePosition(int barPos) const {
        Q_D(const MusicTimeManager);
        if(barPos < 0) {
            qDebug() << "Position of a time signature must be positive.";
            return 0;
        }
        auto it = d->timeSignatureMap.upperBound(barPos);
        if(it == d->timeSignatureMap.end()) {
            return d->timeSignatureMap.lastKey();
        } else {
            return (it - 1).key();
        }
    }
    void MusicTimeManager::removeTimeSignature(int barPos) {
        removeTimeSignatures({barPos});
    }
    void MusicTimeManager::setTempos(const QList<QPair<int, double>> &tempoList) {
    }
    QList<QPair<int, double>> MusicTimeManager::tempos() const {
        Q_D(const MusicTimeManager);
        QList<QPair<int, double>> list;
        for(auto key: d->tempoMap.keys()) {
            list.append(qMakePair(key, d->tempoMap[key]));
        }
        return list;
    }
    void MusicTimeManager::removeTempos(const QList<int> &tickPosList) {
    }
    void MusicTimeManager::setTempo(int tickPos, double tempo) {
        setTempos({{tickPos, tempo}});
    }
    double MusicTimeManager::tempo(int tickPos) const {
        return 0;
    }
    int MusicTimeManager::nearestTempoPosition(int tickPos) const {
        return 0;
    }
    void MusicTimeManager::removeTempo(int tickPos) {
        removeTempos({tickPos});
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
    MusicTime MusicTimeManager::create(double msec) {
        return MusicTime(this, msec);
    }
    MusicTimeManager::MusicTimeManager(MusicTimeManagerPrivate &d, int tpqn): d_ptr(&d) {
        d.q_ptr = this;
        d.tpqn = tpqn;
        d.timeSignatureMap = {{0, {4, 4}}};
        d.tempoMap = {{0, 120}};
        d.measureMap = {{0, 0}};
        d.msecSumMap = {{0, 0}};
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
    }

} // Internal