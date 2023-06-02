#ifndef CHORUSKIT_MUSICTIME_H
#define CHORUSKIT_MUSICTIME_H

#include <QDebug>
#include <QList>
#include <QPair>
#include <QScopedPointer>

namespace ScriptMgr::Internal {

    class MusicTimeline;
    class MusicTimePrivate;

    class MusicTime {
    public:
        ~MusicTime();
        MusicTime(const MusicTime &mt);
        MusicTime &operator=(const MusicTime &mt);

        int measure() const;
        int beat() const;
        int tick() const;
        QString toString() const;

        int totalTick() const;

        double msec() const;

        MusicTimeline *timeline() const;

        friend QDebug operator<<(QDebug debug, MusicTime mt);

        inline bool operator==(const MusicTime &mt) const { return totalTick() == mt.totalTick(); }
        inline bool operator!=(const MusicTime &mt) const { return totalTick() != mt.totalTick(); }
        inline bool operator<(const MusicTime &mt) const { return totalTick() < mt.totalTick(); }
        inline bool operator>(const MusicTime &mt) const { return totalTick() > mt.totalTick(); }
        inline bool operator<=(const MusicTime &mt) const { return totalTick() <= mt.totalTick(); }
        inline bool operator>=(const MusicTime &mt) const { return totalTick() >= mt.totalTick(); }

        inline MusicTime operator+(int t) const {
            return MusicTime(timeline(), 0, 0, totalTick() + t);
        }
        inline MusicTime operator-(int t) const {
            return MusicTime(timeline(), 0, 0, totalTick() - t);
        }

        inline MusicTime& operator+=(int t) {
            return *this = MusicTime(timeline(), 0, 0, totalTick() + t);
        }
        inline MusicTime& operator-=(int t) {
            return *this = MusicTime(timeline(), 0, 0, totalTick() - t);
        }



    private:
        friend class MusicTimeline;
        QSharedPointer<MusicTimePrivate> d_ptr;
        explicit MusicTime(MusicTimePrivate &d, MusicTimeline *timeline);
    protected:
        explicit MusicTime(MusicTimeline *timeline);
        MusicTime(MusicTimeline *timeline, int measure, int beat, int tick);
        MusicTime(MusicTimeline *timeline, const QString &str);
        MusicTime(MusicTimeline *timeline, double msec);
    };
} // Internal

#endif // CHORUSKIT_MUSICTIME_H
