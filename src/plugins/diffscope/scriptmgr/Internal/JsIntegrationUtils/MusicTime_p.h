//
// Created by Crs_1 on 2023/6/1.
//

#ifndef CHORUSKIT_MUSICTIME_P_H
#define CHORUSKIT_MUSICTIME_P_H
#include "MusicTime.h"

namespace ScriptMgr::Internal {

    class MusicTimelinePrivate;

    struct MusicTimeData {
        int totalTick = 0;
        int measure = -1;
        int beat = 0;
        int tick = 0;
        double msec = -1;
        inline bool isMbtNull() const { return measure < 0; }
        inline bool isMsecNull() const { return msec < 0; }
        inline void clearMbt() { measure = -1; }
        inline void clearMsec() { msec = -1; }
    };

    class MusicTimePrivate {
    public:
        MusicTimeline *timeline;
        MusicTimelinePrivate *timeline_d;
        MusicTimeData cache;
        MusicTimePrivate();
        ~MusicTimePrivate();

        void setTimeline(MusicTimeline *timeline_);
        bool setMbt(int measure, int beat, int tick);

        std::tuple<int, int, int> mbt() const;

        bool fromString(const QString &str);

        double msec() const;
        bool fromMsec(double msec);

        void updateCachedMbt();
        void updateCachedMsec();

        void addTimelineMbtCached();
        void removeTimelineMbtCached();

        void addTimelineMsecCached();
        void removeTimelineMsecCached();

    };
}

#endif // CHORUSKIT_MUSICTIME_P_H
