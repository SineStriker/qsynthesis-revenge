#ifndef CHORUSKIT_MUSICTIMELINE_P_H
#define CHORUSKIT_MUSICTIMELINE_P_H

#include "MusicTimeline.h"

namespace ScriptMgr::Internal {

    class MusicTimelinePrivate: public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(MusicTimeline)
    public:
        MusicTimelinePrivate();
        ~MusicTimelinePrivate();
        void init();
        MusicTimeline *q_ptr;
        int tpqn;
        QMap<int, MusicTimeSignature> timeSignatureMap; //bar position -> time signature
        QMap<int, int> measureMap; //tick position -> bar position
        QMap<int, int> revMeasureMap; // bar position -> tick position
        QMap<int, double> tempoMap; //tick position -> tempo
        QMap<int, double> msecSumMap; //tick position -> msecs in total
        QMap<double, int> revMsecSumMap; //msecs in total -> tick position
        void updateMeasureMap(int barFrom);
        void updateMsecSumMap(int tickFrom);
        int findNearestTickWithTimeSignature(int tick) const;
        double findNearestMsecWithTempo(double msec) const;

        QSet<MusicTimePrivate *> mbtCachedMusicTimes;
        QSet<MusicTimePrivate *> msecCachedMusicTimes;
        void _q_timeSignatureChanged();
        void _q_tempoChanged();

    };

} // Internal

#endif // CHORUSKIT_MUSICTIMELINE_P_H
