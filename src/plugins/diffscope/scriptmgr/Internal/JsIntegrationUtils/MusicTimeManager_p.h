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
        int tpqn;
        QMap<int, MusicTimeSignature> timeSignatureMap; //bar position -> time signature
        QMap<int, int> measureMap; //tick position -> bar position
        QMap<int, double> tempoMap; //tick position -> tempo
        QMap<int, double> msecSumMap; //tick position -> msecs in total
        void updateMeasureMap(int barFrom);
    };

} // Internal

#endif // CHORUSKIT_MUSICTIMEMANAGER_P_H
