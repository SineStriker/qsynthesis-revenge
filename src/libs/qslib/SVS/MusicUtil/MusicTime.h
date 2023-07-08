#ifndef MUSICTIME_H
#define MUSICTIME_H

#include <QSharedPointer>

#include "QsSVSGlobal.h"

namespace QsApi {

    class MusicTimeline;

    class MusicTimelinePrivate;

    class PersistentMusicTimeData;

    class MusicTime {
    public:
        Q_DECL_CONSTEXPR inline MusicTime() noexcept;
        Q_DECL_CONSTEXPR inline MusicTime(int measure, int beat, int tick);

        Q_DECL_CONSTEXPR inline int measure() const;
        Q_RELAXED_CONSTEXPR inline void setMeasure(int measure);

        Q_DECL_CONSTEXPR inline int beat() const;
        Q_RELAXED_CONSTEXPR inline void setBeat(int beat);

        Q_DECL_CONSTEXPR inline int tick() const;
        Q_RELAXED_CONSTEXPR inline void setTick(int tick);

        Q_DECL_CONSTEXPR inline bool isValid() const;

    private:
        int m, b, t;

        friend QSSVS_API QDebug operator<<(QDebug debug, const MusicTime &time);
    };

    Q_DECL_CONSTEXPR MusicTime::MusicTime() noexcept : m(0), b(0), t(0) {
    }

    Q_DECL_CONSTEXPR MusicTime::MusicTime(int measure, int beat, int tick) : m(measure), b(beat), t(tick) {
    }

    Q_DECL_CONSTEXPR int MusicTime::measure() const {
        return m;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTime::setMeasure(int measure) {
        m = measure;
    }

    Q_DECL_CONSTEXPR int MusicTime::beat() const {
        return b;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTime::setBeat(int beat) {
        b = beat;
    }

    Q_DECL_CONSTEXPR int MusicTime::tick() const {
        return t;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTime::setTick(int tick) {
        t = tick;
    }

    Q_DECL_CONSTEXPR bool MusicTime::isValid() const {
        return m >= 0 && b >= 0 && t >= 0;
    }

    class QSSVS_API PersistentMusicTime {
    public:
        PersistentMusicTime();
        ~PersistentMusicTime();

        PersistentMusicTime(const PersistentMusicTime &other);
        PersistentMusicTime &operator=(const PersistentMusicTime &other);

        PersistentMusicTime(PersistentMusicTime &&other) noexcept;
        PersistentMusicTime &operator=(PersistentMusicTime &&other) noexcept {
            swap(other);
            return *this;
        }

        void swap(PersistentMusicTime &other) noexcept {
            qSwap(d_ptr, other.d_ptr);
        }

    public:
        const MusicTimeline *timeline() const;

        int measure() const;
        int beat() const;
        int tick() const;

        inline MusicTime toTime() const;

        double msec() const;
        int totalTick() const;

    public:
        inline bool operator==(const PersistentMusicTime &other) const;
        inline bool operator!=(const PersistentMusicTime &other) const;
        inline bool operator<(const PersistentMusicTime &other) const;
        inline bool operator>(const PersistentMusicTime &other) const;
        inline bool operator<=(const PersistentMusicTime &other) const;
        inline bool operator>=(const PersistentMusicTime &other) const;

        PersistentMusicTime operator+(int t) const;
        PersistentMusicTime operator-(int t) const;
        PersistentMusicTime &operator+=(int t);
        PersistentMusicTime &operator-=(int t);

        QString toString() const;

        friend QSSVS_API QDebug operator<<(QDebug debug, const PersistentMusicTime &mt);

    private:
        explicit PersistentMusicTime(PersistentMusicTimeData *d);

        QSharedPointer<PersistentMusicTimeData> d_ptr;

        friend class MusicTimeline;
    };

    MusicTime PersistentMusicTime::toTime() const {
        return MusicTime(measure(), beat(), tick());
    }

    inline bool PersistentMusicTime::operator==(const PersistentMusicTime &other) const {
        return totalTick() == other.totalTick();
    }

    inline bool PersistentMusicTime::operator!=(const PersistentMusicTime &other) const {
        return totalTick() != other.totalTick();
    }

    inline bool PersistentMusicTime::operator<(const PersistentMusicTime &other) const {
        return totalTick() < other.totalTick();
    }

    inline bool PersistentMusicTime::operator>(const PersistentMusicTime &other) const {
        return totalTick() > other.totalTick();
    }

    inline bool PersistentMusicTime::operator<=(const PersistentMusicTime &other) const {
        return totalTick() <= other.totalTick();
    }

    inline bool PersistentMusicTime::operator>=(const PersistentMusicTime &other) const {
        return totalTick() >= other.totalTick();
    }

} // namespace QsApi

#endif // MUSICTIME_H
