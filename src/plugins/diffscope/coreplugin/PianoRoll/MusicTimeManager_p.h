#ifndef CHORUSKIT_MUSICTIMEMANAGER_P_H
#define CHORUSKIT_MUSICTIMEMANAGER_P_H

#include "MusicTimeManager.h"

#include "Window/IProjectWindow.h"

namespace Core {

    class MusicTimeManagerPrivate : public QObject {
        Q_OBJECT
    public:
        MusicTimeManagerPrivate(MusicTimeManager *q);
        ~MusicTimeManagerPrivate();

        MusicTimeManager *q;

        QsApi::MusicTimeline m_timeline;

        int m_currentWidth;
        int m_currentHeight;
        int m_currentSnap;

        struct TimeSignature {
            DspxTimeSignatureEntity *ts;
            int bar;
        };
        QHash<int, TimeSignature> timeSigs;
        DspxTimeSignatureListEntity *timeSigsEntity;

        struct Tempo {
            DspxTempoEntity *t;
            int pos;
        };
        QHash<int, Tempo> tempos;
        DspxTempoListEntity *temposEntity;

    private:
        void _q_timeSignatureInserted(int seq, DspxTimeSignatureEntity *ts);
        void _q_timeSignatureRemoved(int seq);
        void _q_timeSignatureIndexChanged(int bar);
        void _q_timeSignatureValueChanged(int num, int den);

        void _q_tempoInserted(int seq, DspxTempoEntity *t);
        void _q_tempoRemoved(int seq);
        void _q_tempoPosChanged(int pos);
        void _q_tempoValueChanged(double val);

        friend class MusicTimeManager;
    };

}

#endif // CHORUSKIT_MUSICTIMEMANAGER_P_H
