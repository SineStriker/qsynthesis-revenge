#include "MusicTime.h"
#include "MusicTime_p.h"

namespace ScriptMgr::Internal {

    MusicTime::MusicTime() : d(new MusicTimePrivate()) {
    }

    MusicTime::~MusicTime() {
    }

    MusicTime::MusicTime(const QList<QPair<int, double>> &timeSignatures, int tpqn, int measure, int beat, int tick)
        : MusicTime() {
        setTimeSignatures(timeSignatures);
        setTpqn(tpqn);
        setMbt(measure, beat, tick);
    }

    MusicTime::MusicTime(double quartersPerBar, int tpqn, int measure, int beat, int tick)
        : MusicTime(
              {
                  {-std::numeric_limits<int>::lowest(), quartersPerBar}
    },
              tpqn, measure, beat, tick) {
    }

    MusicTime::MusicTime(const QList<QPair<int, double>> &timeSignatures, int tpqn, const QString &str) : MusicTime() {
        // TODO
    }

    MusicTime::MusicTime(double quartersPerBar, int tpqn, const QString &str)
        : MusicTime(
              {
                  {-std::numeric_limits<int>::lowest(), quartersPerBar}
    },
              tpqn, str) {
    }

    void MusicTime::setTimeSignatures(const QList<QPair<int, double>> &timeSignatureList) {
        for (const auto &pair : timeSignatureList) {
            d->timeSig.insert(pair.first, pair.second);
        }
    }

    QList<QPair<int, double>> MusicTime::timeSignatures() const {
        QList<QPair<int, double>> list;
        for (auto iter = d->timeSig.constBegin(); iter != d->timeSig.constEnd(); ++iter) {
            list.append(qMakePair(iter.key(), iter.value()));
        }
        return list;
    }

    void MusicTime::setTimeSignature(int barPos, double quartersPerBar) {
        d->timeSig[barPos] = quartersPerBar;
    }

    void MusicTime::setTimeSignature(double quartersPerBar) {
        setTimeSignature(-std::numeric_limits<int>::lowest(), quartersPerBar);
    }

    double MusicTime::timeSignature(int barPos) const {
        return d->timeSig.value(nearestTimeSignaturePosition(barPos));
    }

    int MusicTime::nearestTimeSignaturePosition(int barPos) const {
        auto followingIter = d->timeSig.upperBound(barPos);
        if (followingIter == d->timeSig.begin())
            return followingIter.key();
        else if (followingIter == d->timeSig.end())
            return d->timeSig.lastKey();
        else
            return (followingIter - 1).key();
    }

    void MusicTime::setTpqn(int tpqn) {
        d->tpqn = tpqn;
    }

    int MusicTime::tpqn() const {
        return d->tpqn;
    }

    bool MusicTime::setMbt(int measure, int beat, int tick) {
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
        return d->measure;
    }

    int MusicTime::beat() const {
        return d->beat;
    }

    int MusicTime::tick() const {
        return d->tick;
    }

    int MusicTime::totalTick() const {
        if (!isValid())
            return 0;
        int boundPos = nearestTimeSignaturePosition(d->measure);
        int totalQuarters = 0;
        if (d->measure >= 0) {
        }

        return 0;
    }

    bool MusicTime::isValid() const {
        // TODO
        return (bool) d;
    }

    void MusicTime::fixToValid() {
    }

    bool MusicTime::isNull() {
        return d->beat == -1 || d->tick == -1;
    }

    QString MusicTime::toString() const {
        // TODO
        return QString();
    }

    void MusicTime::fromString(const QString &str) {
        // TODO
    }

} // Internal