#include "MusicTime.h"
#include <QRegularExpression>
#include <QTextStream>

#include "MusicTime_p.h"
#include "MusicTimeline_p.h"

namespace ScriptMgr::Internal {

    MusicTime::~MusicTime() {

    }

    bool MusicTimePrivate::setMbt(int measure, int beat, int tick) {
        if(measure < 0 || beat < 0 || tick < 0) return false;
        if(measure == 0 && beat == 0) {
            cache.totalTick = tick;
            return true;
        }
        auto timeSig = timeline->timeSignature(measure);
        auto refMeasure = timeline->nearestTimeSignaturePosition(measure);
        auto refTick = timeline_d->revMeasureMap[refMeasure];
        tick += refTick + (measure - refMeasure) * timeSig.ticksPerBar(timeline_d->tpqn);
        tick += beat * timeSig.ticksPerBeat(timeline_d->tpqn);
        cache.totalTick = tick;
        return true;
    }

    std::tuple<int, int, int> MusicTimePrivate::mbt() const {

        int refTick = timeline_d->findNearestTickWithTimeSignature(cache.totalTick);
        int refMeasure = timeline_d->measureMap[refTick];
        MusicTimeSignature timeSig = timeline_d->timeSignatureMap[refMeasure];
        return {
            refMeasure + (cache.totalTick - refTick) / timeSig.ticksPerBar(timeline_d->tpqn),
            ((cache.totalTick - refTick) % timeSig.ticksPerBar(timeline_d->tpqn)) / timeSig.ticksPerBeat(timeline_d->tpqn),
            ((cache.totalTick - refTick) % timeSig.ticksPerBar(timeline_d->tpqn)) % timeSig.ticksPerBeat(timeline_d->tpqn),
        };
    }

    bool MusicTimePrivate::fromString(const QString &str) {
        QRegularExpression rx(R"(^\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*[:\x{ff1a}]?\s*(\d*)\s*$)");
        auto match = rx.match(str);
        if(!match.hasMatch()) return false;
        setMbt(
            match.captured(1).isEmpty() ? 0: (match.captured(1).toInt() - 1),
            match.captured(2).isEmpty() ? 0: (match.captured(2).toInt() - 1),
            match.captured(3).toInt()
        );
        return true;
    }

    double MusicTimePrivate::msec() const {

        auto refTick = timeline->nearestTempoPosition(cache.totalTick);
        auto tempo = timeline_d->tempoMap[refTick];
        auto refMsec = timeline_d->msecSumMap[refTick];
        return refMsec + (cache.totalTick - refTick) * (60.0 * 1000.0) / (timeline_d->tpqn * tempo);
    }

    bool MusicTimePrivate::fromMsec(double msec) {
        if(msec < 0) return false;
        auto refMsec = timeline_d->findNearestMsecWithTempo(msec);
        auto refTick = timeline_d->revMsecSumMap[refMsec];
        auto tempo = timeline_d->tempoMap[refTick];
        auto deltaTick = (int)qRound((msec - refMsec) / (60.0 * 1000.0) * (timeline_d->tpqn * tempo));
        cache.totalTick = refTick + deltaTick;
        return true;
    }

    MusicTimePrivate::MusicTimePrivate() {
    }

    MusicTimePrivate::~MusicTimePrivate() {
        removeTimelineMbtCached();
        removeTimelineMsecCached();
    }

    void MusicTimePrivate::setTimeline(MusicTimeline *timeline_) {
        timeline = timeline_;
        timeline_d = timeline_->d_func();
    }

    void MusicTimePrivate::updateCachedMbt() {
        auto ret = mbt();
        cache.measure = std::get<0>(ret);
        cache.beat = std::get<1>(ret);
        cache.tick = std::get<2>(ret);
        addTimelineMbtCached();
    }

    void MusicTimePrivate::updateCachedMsec() {
        auto ret = msec();
        cache.msec = ret;
        addTimelineMsecCached();
    }

    void MusicTimePrivate::addTimelineMbtCached() {
        timeline_d->mbtCachedMusicTimes.insert(this);
    }

    void MusicTimePrivate::removeTimelineMbtCached() {
        timeline_d->mbtCachedMusicTimes.remove(this);
    }

    void MusicTimePrivate::addTimelineMsecCached() {
        timeline_d->msecCachedMusicTimes.insert(this);
    }

    void MusicTimePrivate::removeTimelineMsecCached() {
        timeline_d->msecCachedMusicTimes.remove(this);
    }





    int MusicTime::measure() const {
        auto d = d_ptr.data();
        if(d->cache.isMbtNull()) {
            d->updateCachedMbt();
        }
        return d->cache.measure;
    }

    int MusicTime::beat() const {
        auto d = d_ptr.data();
        if(d->cache.isMbtNull()) {
            d->updateCachedMbt();
        }
        return d->cache.beat;
    }

    int MusicTime::tick() const {
        auto d = d_ptr.data();
        if(d->cache.isMbtNull()) {
            d->updateCachedMbt();
        }
        return d->cache.tick;
    }

    int MusicTime::totalTick() const {
        auto d = d_ptr.data();
        return d->cache.totalTick;
    }

    double MusicTime::msec() const {
        auto d = d_ptr.data();
        if(d->cache.isMsecNull()) {
            d->updateCachedMsec();
        }
        return d->cache.msec;
    }

    MusicTimeline *MusicTime::timeline() const {
        auto d = d_ptr.data();
        return d->timeline;
    }

    QString MusicTime::toString() const {
        QString str;
        QTextStream textStream(&str);
        textStream << measure() + 1 << ":" << beat() + 1 << ":";
        textStream.setFieldWidth(3);
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream.setPadChar('0');
        textStream << tick();
        textStream.flush();
        return str;
    }

    QDebug operator<<(QDebug debug, MusicTime mt) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTime(";
        debug << "tick=" << mt.totalTick() << ", "
              << "mbt=" << "(" << mt.measure() << ", " << mt.beat() << ", " << mt.tick() << ", " << mt.toString() << "), "
              << "msec=" << mt.msec() << ")";
        return debug;
    }

    MusicTime::MusicTime(MusicTimePrivate &d, MusicTimeline *timeline): d_ptr(&d) {
        d.setTimeline(timeline);
    }
    MusicTime::MusicTime(MusicTimeline *timeline): MusicTime(*new MusicTimePrivate, timeline) {
    }
    MusicTime::MusicTime(MusicTimeline *timeline, int measure, int beat, int tick): MusicTime(timeline) {
        auto d = d_ptr.data();
        d->setMbt(measure, beat, tick);
    }
    MusicTime::MusicTime(MusicTimeline *timeline, const QString &str): MusicTime(timeline) {
        auto d = d_ptr.data();
        d->fromString(str);
    }
    MusicTime::MusicTime(MusicTimeline *timeline, double msec): MusicTime(timeline) {
        auto d = d_ptr.data();
        d->fromMsec(msec);
    }
    MusicTime::MusicTime(const MusicTime &mt) {
        *this = mt;
    }
    MusicTime &MusicTime::operator=(const MusicTime &mt) {
        d_ptr = mt.d_ptr;
        return *this;
    }

} // Internal