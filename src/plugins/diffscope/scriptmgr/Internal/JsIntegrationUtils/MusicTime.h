#ifndef CHORUSKIT_MUSICTIME_H
#define CHORUSKIT_MUSICTIME_H

#include <QList>
#include <QPair>
#include <QScopedPointer>

namespace ScriptMgr::Internal {

    class MusicTimeline;
    class MusicTimePrivate;

    struct MusicTimeData {
        int measure = 0;
        int beat = 0;
        int tick = 0;
        double msec = -1;
    };

    class MusicTime {
        Q_DECLARE_PRIVATE(MusicTime)
    public:
        ~MusicTime();
        MusicTime(const MusicTime &mt);
        MusicTime &operator=(const MusicTime &mt);
        int measure();
        int beat();
        int tick();
        QString toString();

        int totalTick() const;

        double msec();

    private:
        friend class MusicTimeline;
        friend class MusicTimelinePrivate;
        QScopedPointer<MusicTimePrivate> d_ptr;
        int m_tick = 0;
        MusicTimeData m_cache;
        bool m_mbtDirty = true;
        explicit MusicTime(MusicTimePrivate &d, MusicTimeline *timeline);
    protected:
        explicit MusicTime(MusicTimeline *manager);
        MusicTime(MusicTimeline *manager, int measure, int beat, int tick);
        MusicTime(MusicTimeline *manager, const QString &str);
        MusicTime(MusicTimeline *manager, double msec);
    };

} // Internal

#endif // CHORUSKIT_MUSICTIME_H
