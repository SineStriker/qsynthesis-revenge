#include "MusicTime.h"
#include <QRegularExpression>
#include <QTextStream>

#include "MusicTime_p.h"
#include "MusicTimeline_p.h"

namespace ScriptMgr::Internal {

    MusicTime::~MusicTime() {
        Q_D(MusicTime);
        d->removeTimelineMbtCached();
        d->removeTimelineMsecCached();
    }

    bool MusicTimePrivate::setMbt(int measure, int beat, int tick) {
        Q_Q(MusicTime);
        if(measure < 0 || beat < 0 || tick < 0) return false;
        auto timeSig = timeline->timeSignature(measure);
        auto refMeasure = timeline->nearestTimeSignaturePosition(measure);
        auto refTick = timeline_d->revMeasureMap[refMeasure];
        tick += refTick + (measure - refMeasure) * timeSig.ticksPerBar(timeline_d->tpqn);
        tick += beat * timeSig.ticksPerBeat(timeline_d->tpqn);
        q->m_tick = tick;
        return true;
    }

    std::tuple<int, int, int> MusicTimePrivate::mbt() const {
        Q_Q(const MusicTime);
        int refTick = timeline_d->findNearestTickWithTimeSignature(q->m_tick);
        int refMeasure = timeline_d->measureMap[refTick];
        MusicTimeSignature timeSig = timeline_d->timeSignatureMap[refMeasure];
        return {
            refMeasure + (q->m_tick - refTick) / timeSig.ticksPerBar(timeline_d->tpqn),
            ((q->m_tick - refTick) % timeSig.ticksPerBar(timeline_d->tpqn)) / timeSig.ticksPerBeat(timeline_d->tpqn),
            ((q->m_tick - refTick) % timeSig.ticksPerBar(timeline_d->tpqn)) % timeSig.ticksPerBeat(timeline_d->tpqn),
        };
    }

    bool MusicTimePrivate::fromString(const QString &str) {
        QRegularExpression rx(R"(^\s*(\d*)\s*[:\xff1a]?\s*(\d*)\s*[:\xff1a]?\s*(\d*)\s*$)");
        auto match = rx.match(str);
        if(!match.hasMatch()) return false;
        qDebug() << "match: " << match;
        setMbt(
            match.captured(1).isEmpty() ? 0: (match.captured(1).toInt() - 1),
            match.captured(2).isEmpty() ? 0: (match.captured(2).toInt() - 1),
            match.captured(3).toInt()
        );
        return true;
    }

    double MusicTimePrivate::msec() const {
        Q_Q(const MusicTime);
        auto refTick = timeline->nearestTempoPosition(q->m_tick);
        auto tempo = timeline_d->tempoMap[refTick];
        auto refMsec = timeline_d->msecSumMap[refTick];
        return refMsec + (q->m_tick - refTick) * (60.0 * 1000.0) / (timeline_d->tpqn * tempo);
    }

    bool MusicTimePrivate::fromMsec(double msec) {
        Q_Q(MusicTime);
        if(msec < 0) return false;
        auto refMsec = timeline_d->findNearestMsecWithTempo(msec);
        auto refTick = timeline_d->revMsecSumMap[refMsec];
        auto tempo = timeline_d->tempoMap[refTick];
        auto deltaTick = (int)qRound((msec - refMsec) / (60.0 * 1000.0) * (timeline_d->tpqn * tempo));
        q->m_tick = refTick + deltaTick;
        return true;
    }

    MusicTimePrivate::MusicTimePrivate() {
    }

    MusicTimePrivate::~MusicTimePrivate() {
    }

    void MusicTimePrivate::setTimeline(MusicTimeline *timeline_) {
        timeline = timeline_;
        timeline_d = timeline_->d_func();
    }

    void MusicTimePrivate::updateCachedMbt() {
        Q_Q(MusicTime);
        auto ret = mbt();
        q->m_cache.measure = std::get<0>(ret);
        q->m_cache.beat = std::get<1>(ret);
        q->m_cache.tick = std::get<2>(ret);
        addTimelineMbtCached();
    }

    void MusicTimePrivate::updateCachedMsec() {
        Q_Q(MusicTime);
        auto ret = msec();
        q->m_cache.msec = ret;
        addTimelineMsecCached();
    }

    void MusicTimePrivate::addTimelineMbtCached() {
        Q_Q(MusicTime);
        timeline_d->mbtCachedMusicTimes.insert(q);
    }

    void MusicTimePrivate::removeTimelineMbtCached() {
        Q_Q(MusicTime);
        timeline_d->mbtCachedMusicTimes.remove(q);
    }

    void MusicTimePrivate::addTimelineMsecCached() {
        Q_Q(MusicTime);
        timeline_d->msecCachedMusicTimes.insert(q);
    }

    void MusicTimePrivate::removeTimelineMsecCached() {
        Q_Q(MusicTime);
        timeline_d->msecCachedMusicTimes.remove(q);
    }





    int MusicTime::measure() {
        Q_D(MusicTime);
        if(m_cache.isMbtNull()) {
            d->updateCachedMbt();
        }
        return m_cache.measure;
    }

    int MusicTime::beat() {
        Q_D(MusicTime);
        if(m_cache.isMbtNull()) {
            d->updateCachedMbt();
        }
        return m_cache.beat;
    }

    int MusicTime::tick() {
        Q_D(MusicTime);
        if(m_cache.isMbtNull()) {
            d->updateCachedMbt();
        }
        return m_cache.tick;
    }

    int MusicTime::totalTick() const {
        return m_tick;
    }

    double MusicTime::msec() {
        Q_D(MusicTime);
        if(m_cache.isMsecNull()) {
            d->updateCachedMsec();
        }
        return m_cache.msec;
    }

    QString MusicTime::toString() {
        QString str;
        QTextStream textStream(&str);
        textStream << measure() + 1 << ":" << beat() + 1 << ":";
        textStream.setFieldWidth(3);
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream.setPadChar('0');
        textStream << tick();
        return str;
    }

    QDebug operator<<(QDebug debug, MusicTime mt) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTime(";
        debug << "tick=" << mt.m_tick << ", "
              << "mbt=" << "(" << mt.measure() << ", " << mt.beat() << ", " << mt.tick() << ", " << mt.toString() << "), "
              << "msec=" << mt.msec() << ")";
        return debug;
    }

    MusicTime::MusicTime(MusicTimePrivate &d, MusicTimeline *timeline): d_ptr(&d) {
        d.q_ptr = this;
        d.setTimeline(timeline);
    }
    MusicTime::MusicTime(MusicTimeline *manager): MusicTime(*new MusicTimePrivate, manager) {
    }
    MusicTime::MusicTime(MusicTimeline *manager, int measure, int beat, int tick): MusicTime(manager) {
        Q_D(MusicTime);
        d->setMbt(measure, beat, tick);
    }
    MusicTime::MusicTime(MusicTimeline *manager, const QString &str): MusicTime(manager) {
        Q_D(MusicTime);
        d->fromString(str);
    }
    MusicTime::MusicTime(MusicTimeline *manager, double msec): MusicTime(manager) {
        Q_D(MusicTime);
        d->fromMsec(msec);
    }
    MusicTime::MusicTime(const MusicTime &mt): MusicTime(mt.d_func()->timeline) {
        *this = mt;
    }
    MusicTime &MusicTime::operator=(const MusicTime &mt) {
        Q_D(MusicTime);
        d->setTimeline(mt.d_func()->timeline);
        m_tick = mt.m_tick;
        if(!mt.m_cache.isMbtNull()) {
            m_cache = mt.m_cache;
            d->addTimelineMbtCached();
        } else {
            m_cache.clearMbt();
            d->removeTimelineMbtCached();
        }
        if(!mt.m_cache.isMsecNull()) {
            m_cache.msec = mt.m_cache.msec;
            d->addTimelineMsecCached();
        } else {
            m_cache.clearMsec();
            d->removeTimelineMsecCached();
        }
        return *this;
    }

} // Internal