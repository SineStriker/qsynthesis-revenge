#ifndef CHORUSKIT_MUSICTIMEMANAGER_P_H
#define CHORUSKIT_MUSICTIMEMANAGER_P_H

#include "MusicTimeManager.h"

namespace ScriptMgr::Internal {

    class MusicTimeManagerPrivate {
        Q_DECLARE_PUBLIC(MusicTimeManager)
    public:
        MusicTimeManagerPrivate();
        ~MusicTimeManagerPrivate();
        MusicTimeManager *q_ptr;
        tick_t tpqn;
        QMap<measure_t, MusicTimeSignature> timeSignatureMap; //bar position -> time signature
        QMap<tick_t, measure_t> measureMap; //tick position -> bar position
        QMap<measure_t, tick_t> revMeasureMap; // bar position -> tick position
        QMap<tick_t, tempo_t> tempoMap; //tick position -> tempo
        QMap<tick_t, msec_t> msecSumMap; //tick position -> msecs in total
        QMap<msec_t, tick_t> revMsecSumMap; //msecs in total -> tick position
        void updateMeasureMap(measure_t barFrom);
        void updateMsecSumMap(tick_t tickFrom);
        tick_t findNearestTickWithTimeSignature(tick_t tick) const;
        msec_t findNearestMsecWithTempo(msec_t msec) const;
    };

} // Internal

#endif // CHORUSKIT_MUSICTIMEMANAGER_P_H
