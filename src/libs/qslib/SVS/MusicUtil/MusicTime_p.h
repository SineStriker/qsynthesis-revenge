#ifndef MUSICTIMEPRIVATE_H
#define MUSICTIMEPRIVATE_H

#include <QPointer>

#include "MusicTime.h"

namespace QsApi {

    class MusicTimelinePrivate;

    struct MusicTimeCache {
        MusicTimeCache() : measure(-1), beat(0), tick(0), msec(-1) {
        }

        inline bool isMbtNull() const {
            return measure < 0;
        }

        inline bool isMsecNull() const {
            return msec < 0;
        }

        inline void clearMbt() {
            measure = -1;
        }

        inline void clearMsec() {
            msec = -1;
        }

        int measure;
        int beat;
        int tick;
        double msec;
    };

    class PersistentMusicTimeData {
    public:
        PersistentMusicTimeData(const MusicTimeline *timeline, const MusicTimelinePrivate *td, int totalTick);
        ~PersistentMusicTimeData();

        const MusicTimeline *timeline;
        const MusicTimelinePrivate *td;

        int totalTick;
        MusicTimeCache cache;

        void ensureMbtCached();
        void ensureMsecCached();
    };

}

#endif // MUSICTIMEPRIVATE_H