//
// Created by Crs_1 on 2023/6/1.
//

#ifndef CHORUSKIT_MUSICTIME_P_H
#define CHORUSKIT_MUSICTIME_P_H
#include "MusicTime.h"

namespace ScriptMgr::Internal {

    class MusicTimePrivate {
        Q_DECLARE_PUBLIC(MusicTime)
    public:
        MusicTime *q_ptr;
        MusicTimeline *timeline;
        MusicTimelinePrivate *timeline_d;
        MusicTimePrivate();
        ~MusicTimePrivate();

        void init(MusicTimeline *timeline_);
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
