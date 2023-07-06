#include "MusicTimeline.h"
#include "MusicTimeline_p.h"

#include "MusicTime_p.h"

#include <QDataStream>
#include <QDebug>
#include <QRegularExpression>

namespace QsApi {

    QDataStream &operator<<(QDataStream &out, const MusicTimeSignature &ts) {
        out << ts.numerator() << ts.denominator();
        return out;
    }

    QDataStream &operator>>(QDataStream &in, MusicTimeSignature &ts) {
        int n, d;
        in >> n >> d;
        ts = MusicTimeSignature(n, d);
        return in;
    }

    //===========================================================================
    // MusicTimeSignature
    static bool m_registered = false;

    static void registerMetaType() {
        qRegisterMetaType<MusicTimeSignature>();
        qRegisterMetaTypeStreamOperators<MusicTimeSignature>();
        m_registered = true;
    }

    Q_DECL_CONSTEXPR MusicTimeSignature::MusicTimeSignature() : MusicTimeSignature(4, 4) {
    }

    Q_DECL_CONSTEXPR MusicTimeSignature::MusicTimeSignature(int numerator, int denominator)
        : num(numerator), den(denominator) {
        if (!m_registered)
            registerMetaType();
    }

    Q_DECL_CONSTEXPR bool MusicTimeSignature::isValid() const {
        if (num <= 0)
            return false;
        if (den < 1 || den > 32)
            return false;
        return !(den & (den - 1));
    }

    Q_DECL_CONSTEXPR int MusicTimeSignature::ticksPerBar(int resolution) const {
        return resolution * num * 4 / den;
    }

    Q_DECL_CONSTEXPR int MusicTimeSignature::ticksPerBeat(int resolution) const {
        return resolution * 4 / den;
    }

    QString MusicTimeSignature::toString() const {
        return QString::asprintf("%d/%d", num, den);
    }

    QDebug operator<<(QDebug debug, const MusicTimeSignature &t) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTimeSignature(" << t.num << "/" << t.den << ")";
        return debug;
    }
    //===========================================================================

    //===========================================================================
    // MusicTimeline
    MusicTimelinePrivate::~MusicTimelinePrivate() {
    }

    void MusicTimelinePrivate::init() {
        timeSignatureMap = {{0, {4, 4}}};
        tempoMap = {{0, 120}};
        measureMap = {{0, 0}};
        revMeasureMap = {{0, 0}};
        msecSumMap = {{0, 0}};
        revMsecSumMap = {{0, 0}};
    }

    void MusicTimelinePrivate::updateMeasureMap(int barFrom) {
        auto iter = timeSignatureMap.lowerBound(barFrom);
        if (iter == timeSignatureMap.begin())
            iter++;
        for (auto it = measureMap.upperBound(revMeasureMap[(iter - 1).key()]);
             it != measureMap.end();) {
            revMeasureMap.remove(it.value());
            it = measureMap.erase(it);
        }
        for (; iter != timeSignatureMap.end(); iter++) {
            auto prevMeasure = (iter - 1).key();
            auto prevTimeSig = (iter - 1).value();
            auto prevTickPos = revMeasureMap[prevMeasure];
            auto currentTickPos =
                prevTickPos + (iter.key() - prevMeasure) * prevTimeSig.ticksPerBar(resolution);
            measureMap[currentTickPos] = iter.key();
            revMeasureMap[iter.key()] = currentTickPos;
        }
    }

    void MusicTimelinePrivate::updateMsecSumMap(int tickFrom) {
        auto iter = tempoMap.lowerBound(tickFrom);
        if (iter == tempoMap.begin())
            iter++;
        for (auto it = msecSumMap.upperBound((iter - 1).key()); it != msecSumMap.end();) {
            revMsecSumMap.remove(it.value());
            it = msecSumMap.erase(it);
        }
        for (; iter != tempoMap.end(); iter++) {
            auto prevTick = (iter - 1).key();
            auto prevTempo = (iter - 1).value();
            auto prevMsec = msecSumMap[prevTick];
            auto currentMsec =
                prevMsec + (iter.key() - prevTick) * (60.0 * 1000.0) / (resolution * prevTempo);
            msecSumMap[iter.key()] = currentMsec;
            revMsecSumMap[currentMsec] = iter.key();
        }
    }

    void MusicTimelinePrivate::timeSignatureChanged() {
        Q_Q(MusicTimeline);
        for (auto t : qAsConst(mbtCachedMusicTimes)) {
            t->cache.clearMbt();
        }
        mbtCachedMusicTimes.clear();

        emit q->timeSignatureChanged();
        emit q->changed();
    }

    void MusicTimelinePrivate::tempoChanged() {
        Q_Q(MusicTimeline);
        for (auto t : qAsConst(msecCachedMusicTimes)) {
            t->cache.clearMsec();
        }
        msecCachedMusicTimes.clear();

        emit q->tempoChanged();
        emit q->changed();
    }

    int MusicTimelinePrivate::findNearestTickWithTimeSignature(int tick) const {
        if (tick < 0)
            return 0;
        auto it = measureMap.upperBound(tick);
        if (it != measureMap.end()) {
            return (it - 1).key();
        }
        return measureMap.lastKey();
    }

    double MusicTimelinePrivate::findNearestMsecWithTempo(double msec) const {
        if (msec < 0)
            return 0;
        auto it = revMsecSumMap.upperBound(msec);
        if (it != revMsecSumMap.end()) {
            return (it - 1).key();
        }
        return revMsecSumMap.lastKey();
    }

    MusicTimeline::MusicTimeline(QObject *parent)
        : MusicTimeline(*new MusicTimelinePrivate(), parent) {
    }

    MusicTimeline::~MusicTimeline() {
    }

    void MusicTimeline::addTimeSignatures(
        const QList<QPair<int, MusicTimeSignature>> &timeSignatureList) {
        Q_D(MusicTimeline);

        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for (const auto &pair : timeSignatureList) {
            if (pair.first < 0) {
                qWarning() << "Position of a time signature must be positive or zero, but read"
                           << pair.first << ".";
                continue;
            }
            if (d->timeSignatureMap.contains(pair.first) &&
                d->timeSignatureMap[pair.first] == pair.second) {
                // identical to current time signature
                continue;
            }
            if (!pair.second.isValid()) {
                qWarning() << "Invalid time signature '" << pair.second.toString() << "'.";
                continue;
            }
            isChanged = true;
            d->timeSignatureMap[pair.first] = pair.second;
            minPos = qMin(minPos, pair.first);
        }

        if (isChanged) {
            d->updateMeasureMap(minPos);
            d->timeSignatureChanged();
        }
    }

    void MusicTimeline::removeTimeSignatures(const QList<int> &bars) {
        Q_D(MusicTimeline);

        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for (auto barPos : bars) {
            if (barPos <= 0) {
                qWarning() << "Position of a time signature must be positive, but read" << barPos
                           << ".";
                continue;
            }
            if (!d->timeSignatureMap.contains(barPos)) {
                qWarning() << "Bar" << barPos << "does not have time signature.";
                continue;
            }
            isChanged = true;
            d->timeSignatureMap.remove(barPos);
            minPos = qMin(minPos, barPos);
        }
        if (isChanged) {
            d->updateMeasureMap(minPos);
            d->timeSignatureChanged();
        }
    }

    QList<QPair<int, MusicTimeSignature>> MusicTimeline::timeSignatures() const {
        Q_D(const MusicTimeline);
        QList<QPair<int, MusicTimeSignature>> list;
        for (auto it = d->timeSignatureMap.begin(); it != d->timeSignatureMap.end(); ++it) {
            list.append({it.key(), it.value()});
        }
        return list;
    }

    QList<int> MusicTimeline::timeSignatureBars() const {
        Q_D(const MusicTimeline);
        return d->timeSignatureMap.keys();
    }

    MusicTimeSignature MusicTimeline::timeSignature(int bar) const {
        Q_D(const MusicTimeline);
        return d->timeSignatureMap.value(nearestTimeSignatureBar(bar));
    }

    int MusicTimeline::nearestTimeSignatureBar(int bar) const {
        Q_D(const MusicTimeline);
        if (bar < 0) {
            qWarning() << "Position of a time signature must be positive or zero.";
            return 0;
        }
        auto it = d->timeSignatureMap.upperBound(bar);
        if (it != d->timeSignatureMap.end()) {
            return (it - 1).key();
        }
        return d->timeSignatureMap.lastKey();
    }

    void MusicTimeline::addTempos(const QList<QPair<int, double>> &tempos) {
        Q_D(MusicTimeline);

        bool isChanged = false;
        int minPos = std::numeric_limits<int>::max();
        for (const auto &pair : tempos) {
            if (pair.first < 0) {
                qWarning() << "Position of a tempo must be positive or zero, but read "
                           << pair.first << " .";
                continue;
            }
            if (d->msecSumMap.contains(pair.first) && d->msecSumMap[pair.first] == pair.second) {
                // identical to current time signature
                continue;
            }
            if (pair.second <= 0) {
                qWarning() << "Invalid tempo '" << pair.second << "'.";
                continue;
            }
            isChanged = true;
            d->tempoMap[pair.first] = pair.second;
            minPos = qMin(minPos, pair.first);
        }
        if (isChanged) {
            d->updateMsecSumMap(minPos);
            d->tempoChanged();
        }
    }

    void MusicTimeline::removeTempos(const QList<int> &ticks) {
        Q_D(MusicTimeline);
        bool isChanged = false;

        int minPos = std::numeric_limits<int>::max();
        for (const auto tickPos : ticks) {
            if (tickPos <= 0) {
                qWarning() << "Position of a tempo must be positive, but read" << tickPos << ".";
                continue;
            }
            if (!d->msecSumMap.contains(tickPos)) {
                qWarning() << "Tick" << tickPos << "does not have tempo.";
                continue;
            }
            isChanged = true;
            d->tempoMap.remove(tickPos);
            minPos = qMin(minPos, tickPos);
        }
        if (isChanged) {
            d->updateMsecSumMap(minPos);
            d->tempoChanged();
        }
    }

    QList<QPair<int, double>> MusicTimeline::tempos() const {
        Q_D(const MusicTimeline);
        QList<QPair<int, double>> list;
        for (auto it = d->tempoMap.begin(); it != d->tempoMap.end(); ++it) {
            list.append({it.key(), it.value()});
        }
        return list;
    }

    QList<int> MusicTimeline::tempoTicks() const {
        Q_D(const MusicTimeline);
        return d->tempoMap.keys();
    }

    double MusicTimeline::tempo(int tick) const {
        Q_D(const MusicTimeline);
        return d->tempoMap.value(nearestTempoTick(tick));
    }

    int MusicTimeline::nearestTempoTick(int tick) const {
        Q_D(const MusicTimeline);
        if (tick < 0) {
            qWarning() << "Position of a tempo must be positive or zero.";
            return 0;
        }
        auto it = d->tempoMap.upperBound(tick);
        if (it != d->tempoMap.end()) {
            return (it - 1).key();
        }
        return d->tempoMap.lastKey();
    }


    MusicTime MusicTimeline::tickToTime(int totalTick) const {
        Q_D(const MusicTimeline);
        int refTick = d->findNearestTickWithTimeSignature(totalTick);
        int refMeasure = d->measureMap[refTick];
        MusicTimeSignature timeSig = d->timeSignatureMap[refMeasure];
        return {
            refMeasure + (totalTick - refTick) / timeSig.ticksPerBar(d->resolution),
            ((totalTick - refTick) % timeSig.ticksPerBar(d->resolution)) /
                timeSig.ticksPerBeat(d->resolution),
            ((totalTick - refTick) % timeSig.ticksPerBar(d->resolution)) %
                timeSig.ticksPerBeat(d->resolution),
        };
    }

    double MusicTimeline::tickToMsec(int totalTick) const {
        Q_D(const MusicTimeline);
        auto refTick = nearestTempoTick(totalTick);
        auto tempo = d->tempoMap[refTick];
        auto refMsec = d->msecSumMap[refTick];
        return refMsec + (totalTick - refTick) * (60.0 * 1000.0) / (d->resolution * tempo);
    }

    int MusicTimeline::timeToTick(int measure, int beat, int tick) const {
        Q_D(const MusicTimeline);
        if (measure < 0 || beat < 0 || tick < 0)
            return 0;
        if (measure == 0 && beat == 0) {
            return tick;
        }

        auto timeSig = timeSignature(measure);
        auto refMeasure = nearestTimeSignatureBar(measure);
        auto refTick = d->revMeasureMap[refMeasure];
        tick += refTick + (measure - refMeasure) * timeSig.ticksPerBar(d->resolution);
        tick += beat * timeSig.ticksPerBeat(d->resolution);
        return tick;
    }

    int MusicTimeline::stringToTick(const QString &str) const {
        QRegularExpression rx(R"(^\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*$)");
        auto match = rx.match(str);
        if (!match.hasMatch())
            return false;
        return timeToTick(match.captured(1).isEmpty() ? 0 : (match.captured(1).toInt() - 1),
                          match.captured(2).isEmpty() ? 0 : (match.captured(2).toInt() - 1),
                          match.captured(3).toInt());
    }

    int MusicTimeline::msecToTick(double msec) const {
        Q_D(const MusicTimeline);
        if (msec < 0)
            return false;
        auto refMsec = d->findNearestMsecWithTempo(msec);
        auto refTick = d->revMsecSumMap[refMsec];
        auto tempo = d->tempoMap[refTick];
        auto deltaTick = (int) qRound((msec - refMsec) / (60.0 * 1000.0) * (d->resolution * tempo));
        return refTick + deltaTick;
    }

    PersistentMusicTime MusicTimeline::create(int measure, int beat, int tick) const {
        Q_D(const MusicTimeline);
        auto container = new PersistentMusicTimeData(this, d, timeToTick(measure, beat, tick));
        return PersistentMusicTime(container);
    }

    PersistentMusicTime MusicTimeline::create(const QString &str) const {
        Q_D(const MusicTimeline);
        auto container = new PersistentMusicTimeData(this, d, stringToTick(str));
        return PersistentMusicTime(container);
    }

    PersistentMusicTime MusicTimeline::create(double msec) const {
        Q_D(const MusicTimeline);
        auto container = new PersistentMusicTimeData(this, d, msecToTick(msec));
        return PersistentMusicTime(container);
    }

    MusicTimelinePrivate::MusicTimelinePrivate() {
        timeSignatureMap = {{0, {4, 4}}};
        tempoMap = {{0, 120}};
        measureMap = {{0, 0}};
        revMeasureMap = {{0, 0}};
        msecSumMap = {{0, 0}};
        revMsecSumMap = {{0, 0}};
    }

    MusicTimeline::MusicTimeline(MusicTimelinePrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
    //===========================================================================

} // namespace QsApi
