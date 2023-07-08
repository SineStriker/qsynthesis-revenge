#include "MusicTime.h"
#include "MusicTime_p.h"

#include "MusicTimeline_p.h"

#include <QDebugStateSaver>
#include <QRegularExpression>
#include <QTextStream>

namespace QsApi {

    QDebug operator<<(QDebug debug, const MusicTime &time) {
        QDebugStateSaver saver(debug);
        qDebug().noquote() << "MusicTime(" << time.m << ", " << time.b << ", " << time.t << ")";
        return debug;
    }

    PersistentMusicTimeData::PersistentMusicTimeData(const MusicTimeline *timeline, const MusicTimelinePrivate *td,
                                                     int totalTick)
        : timeline(timeline), td(td), totalTick(totalTick) {
    }

    PersistentMusicTimeData::~PersistentMusicTimeData() {
        td->mbtCachedMusicTimes.remove(this);
        td->msecCachedMusicTimes.remove(this);
    }

    void PersistentMusicTimeData::ensureMbtCached() {
        if (!cache.isMbtNull())
            return;

        auto ret = timeline->tickToTime(totalTick);
        cache.measure = ret.measure();
        cache.beat = ret.beat();
        cache.tick = ret.tick();
        td->mbtCachedMusicTimes.insert(this);
    }

    void PersistentMusicTimeData::ensureMsecCached() {
        if (!cache.isMsecNull())
            return;
        cache.msec = timeline->tickToMsec(totalTick);
        td->msecCachedMusicTimes.insert(this);
    }

    PersistentMusicTime::PersistentMusicTime() {
    }

    PersistentMusicTime::~PersistentMusicTime() {
    }

    PersistentMusicTime::PersistentMusicTime(const PersistentMusicTime &other) {
        *this = other;
    }

    PersistentMusicTime &PersistentMusicTime::operator=(const PersistentMusicTime &other) {
        if (this == &other)
            return *this;
        d_ptr = other.d_ptr;
        return *this;
    }

    PersistentMusicTime::PersistentMusicTime(QsApi::PersistentMusicTime &&other) noexcept {
        swap(other);
    }

    const MusicTimeline *PersistentMusicTime::timeline() const {
        auto d = d_ptr.data();
        if (!d)
            return nullptr;
        return d->timeline;
    }

    int PersistentMusicTime::measure() const {
        auto d = d_ptr.data();
        if (!d)
            return 0;
        d->ensureMbtCached();
        return d->cache.measure;
    }

    int PersistentMusicTime::beat() const {
        auto d = d_ptr.data();
        if (!d)
            return 0;
        d->ensureMbtCached();
        return d->cache.beat;
    }

    int PersistentMusicTime::tick() const {
        auto d = d_ptr.data();
        if (!d)
            return 0;
        d->ensureMbtCached();
        return d->cache.tick;
    }

    double PersistentMusicTime::msec() const {
        auto d = d_ptr.data();
        if (!d)
            return 0;
        d->ensureMsecCached();
        return d->cache.msec;
    }

    int PersistentMusicTime::totalTick() const {
        auto d = d_ptr.data();
        if (!d)
            return 0;
        return d->totalTick;
    }

    PersistentMusicTime PersistentMusicTime::operator+(int t) const {
        auto d = d_ptr.data();
        if (!d)
            return {};
        return d->timeline->create(0, 0, d->totalTick + t);
    }

    PersistentMusicTime PersistentMusicTime::operator-(int t) const {
        auto d = d_ptr.data();
        if (!d)
            return {};
        return d->timeline->create(0, 0, d->totalTick - t);
    }

    PersistentMusicTime &PersistentMusicTime::operator+=(int t) {
        return (*this = *this + t);
    }

    PersistentMusicTime &PersistentMusicTime::operator-=(int t) {
        return (*this = *this - t);
    }

    QString PersistentMusicTime::toString() const {
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

    QDebug operator<<(QDebug debug, const PersistentMusicTime &mt) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTime(";
        debug << "tick=" << mt.totalTick() << ", "
              << "mbt="
              << "(" << mt.measure() << ", " << mt.beat() << ", " << mt.tick() << ", " << mt.toString() << "), "
              << "msec=" << mt.msec() << ")";
        return debug;
    }

    PersistentMusicTime::PersistentMusicTime(QsApi::PersistentMusicTimeData *d) : d_ptr(d) {
    }

}
