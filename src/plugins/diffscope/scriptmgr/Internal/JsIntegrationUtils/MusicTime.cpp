#include "MusicTime.h"
#include "MusicTime_p.h"

namespace ScriptMgr::Internal {

    MusicTime::MusicTime(): MusicTime(new MusicTimePrivate) {
    }
    MusicTime::MusicTime(const QList<QPair<int, double>> &timeSignatures, int tpqn, int measure, int beat, int tick): MusicTime(new MusicTimePrivate) {
          setTimeSignatures(timeSignatures);
          setTpqn(tpqn);
          setMbt(measure, beat, tick);
    }
    MusicTime::MusicTime(double quartersPerBar, int tpqn, int measure, int beat, int tick): MusicTime({{-std::numeric_limits<int>::lowest(), quartersPerBar}}, tpqn, measure, beat, tick) {
    }
    MusicTime::MusicTime(const QList<QPair<int, double>> &timeSignatures, int tpqn, const QString &str): MusicTime(new MusicTimePrivate) {
          //TODO
    }
    MusicTime::MusicTime(double quartersPerBar, int tpqn, const QString &str): MusicTime({{-std::numeric_limits<int>::lowest(), quartersPerBar}}, tpqn, str) {
    }
    void MusicTime::setTimeSignatures(const QList<QPair<int, double>> &timeSignatureList) {
        Q_D(MusicTime);
        for(const auto &pair: timeSignatureList) {
            d->timeSig.insert(pair.first, pair.second);
        }
    }
    QList<QPair<int, double>> MusicTime::timeSignatures() const {
        Q_D(const MusicTime);
        QList<QPair<int, double>> list;
        for (auto iter = d->timeSig.constBegin(); iter != d->timeSig.constEnd(); ++iter) {
            list.append(qMakePair(iter.key(), iter.value()));
        }
        return list;
    }
    void MusicTime::setTimeSignature(int barPos, double quartersPerBar) {
        Q_D(MusicTime);
        d->timeSig[barPos] = quartersPerBar;
    }
    void MusicTime::setTimeSignature(double quartersPerBar) {
        setTimeSignature(-std::numeric_limits<int>::lowest(), quartersPerBar);
    }
    double MusicTime::timeSignature(int barPos) const {
        Q_D(const MusicTime);
        return d->timeSig.value(nearestTimeSignaturePosition(barPos));
    }
    int MusicTime::nearestTimeSignaturePosition(int barPos) const {
        Q_D(const MusicTime);
        auto followingIter = d->timeSig.upperBound(barPos);
        if(followingIter == d->timeSig.begin()) return followingIter.key();
        else if(followingIter == d->timeSig.end()) return d->timeSig.lastKey();
        else return (followingIter - 1).key();
    }
    void MusicTime::setTpqn(int tpqn) {
        Q_D(MusicTime);
        d->tpqn = tpqn;
    }
    int MusicTime::tpqn() const {
        Q_D(const MusicTime);
        return d->tpqn;
    }
    bool MusicTime::setMbt(int measure, int beat, int tick) {
        Q_D(MusicTime);
        d->measure = measure;
        beat = std::min(0, beat);
        tick = std::min(0, tick);
        d->beat = beat;
        d->tick = tick;
        return isValid();
    }
    void MusicTime::setTotalTick(int tick) {
    }
    int MusicTime::measure() const {
        Q_D(const MusicTime);
        return d->measure;
    }
    int MusicTime::beat() const {
        Q_D(const MusicTime);
        return d->beat;
    }
    int MusicTime::tick() const {
        Q_D(const MusicTime);
        return d->tick;
    }
    int MusicTime::totalTick() const {
        Q_D(const MusicTime);
        if(!isValid()) return 0;
        int boundPos = nearestTimeSignaturePosition(d->measure);
        int totalQuarters = 0;
        if(d->measure >= 0) {

        }

        return 0;
    }
    bool MusicTime::isValid() const {
        //TODO
        Q_D(const MusicTime);
        return (bool)d;
    }
    void MusicTime::fixToValid() {
    }
    bool MusicTime::isNull() {
        Q_D(const MusicTime);
        return d->beat == -1 || d->tick == -1;
    }
    QString MusicTime::toString() const {
        //TODO
        return QString();
    }
    void MusicTime::fromString(const QString &str) {
        //TODO
    }
    MusicTime::MusicTime(MusicTimePrivate *d): d_ptr(d) {
        d->tpqn = 480;
        d->beat = -1;
        d->tick = -1;
        d->timeSig.insert(-std::numeric_limits<int>::lowest(), 4);
    }

} // Internal