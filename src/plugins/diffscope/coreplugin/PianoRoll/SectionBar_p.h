#ifndef SECTIONBARPRIVATE_H
#define SECTIONBARPRIVATE_H

#include "SectionBar.h"

#include "Window/IProjectWindow.h"

namespace Core {

    class SectionBarPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(SectionBar)
    public:
        SectionBarPrivate();
        virtual ~SectionBarPrivate();

        void init();

        SectionBar *q_ptr;

        QsApi::MusicTimeline timeline;

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
        void q_timeSignatureIndexChanged(int bar);
        void _q_timeSignatureValueChanged(int num, int den);

        void _q_tempoInserted(int seq, DspxTempoEntity *t);
        void _q_tempoRemoved(int seq);
        void _q_tempoPosChanged(int pos);
        void _q_tempoValueChanged(double val);
    };

} // namespace Core

#endif // SECTIONBARPRIVATE_H
