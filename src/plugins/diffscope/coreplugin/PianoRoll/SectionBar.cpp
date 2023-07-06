#include "SectionBar.h"
#include "SectionBar_p.h"

namespace Core {

    SectionBarPrivate::SectionBarPrivate() {
        timeSigsEntity = nullptr;
        temposEntity = nullptr;
    }

    SectionBarPrivate::~SectionBarPrivate() {
    }

    void SectionBarPrivate::init() {
    }

    void SectionBarPrivate::_q_timeSignatureInserted(int seq, DspxTimeSignatureEntity *ts) {
        timeSigs.insert(seq, {ts, ts->barIndex()});
        timeline.addTimeSignature(ts->barIndex(), ts->value());

        connect(ts, &DspxTimeSignatureEntity::barIndexChanged, this, &SectionBarPrivate::q_timeSignatureIndexChanged);
        connect(ts, &DspxTimeSignatureEntity::valueChanged, this, &SectionBarPrivate::_q_timeSignatureValueChanged);
    }

    void SectionBarPrivate::_q_timeSignatureRemoved(int seq) {
        auto it = timeSigs.find(seq);
        auto &ts = it.value().ts;

        disconnect(ts, &DspxTimeSignatureEntity::barIndexChanged, this,
                   &SectionBarPrivate::q_timeSignatureIndexChanged);
        disconnect(ts, &DspxTimeSignatureEntity::valueChanged, this, &SectionBarPrivate::_q_timeSignatureValueChanged);

        timeSigs.erase(it);
    }

    void SectionBarPrivate::q_timeSignatureIndexChanged(int bar) {
        Q_Q(SectionBar);

        auto entity = static_cast<DspxTimeSignatureEntity *>(sender());
        auto seq = timeSigsEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto ts = timeSigs.value(seq);

        // Remove old time signature
        timeline.removeTimeSignature(ts.bar);

        ts.bar = bar;

        // Insert new time signature
        timeline.addTimeSignature(bar, ts.ts->value());
    }

    void SectionBarPrivate::_q_timeSignatureValueChanged(int num, int den) {
        auto entity = static_cast<DspxTimeSignatureEntity *>(sender());
        auto seq = timeSigsEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto ts = timeSigs.value(seq);

        // Replace time signature
        timeline.addTimeSignature(ts.bar, ts.ts->value());
    }

    void SectionBarPrivate::_q_tempoInserted(int seq, DspxTempoEntity *t) {
        tempos.insert(seq, {t, t->position()});
        timeline.addTempo(t->position(), t->value());

        connect(t, &DspxTempoEntity::positionChanged, this, &SectionBarPrivate::_q_tempoPosChanged);
        connect(t, &DspxTempoEntity::valueChanged, this, &SectionBarPrivate::_q_tempoValueChanged);
    }

    void SectionBarPrivate::_q_tempoRemoved(int seq) {
        auto it = tempos.find(seq);
        auto &t = it.value().t;

        disconnect(t, &DspxTempoEntity::positionChanged, this, &SectionBarPrivate::_q_tempoPosChanged);
        disconnect(t, &DspxTempoEntity::valueChanged, this, &SectionBarPrivate::_q_tempoValueChanged);

        tempos.erase(it);
    }

    void SectionBarPrivate::_q_tempoPosChanged(int pos) {
        auto entity = static_cast<DspxTempoEntity *>(sender());
        auto seq = temposEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto t = tempos.value(seq);

        // Remove old tempo
        timeline.removeTempo(t.pos);

        t.pos = pos;

        // Insert new tempo
        timeline.addTempo(pos, t.t->value());
    }

    void SectionBarPrivate::_q_tempoValueChanged(double val) {
        auto entity = static_cast<DspxTempoEntity *>(sender());
        auto seq = temposEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto t = tempos.value(seq);

        // Replace tempo
        timeline.addTempo(t.pos, t.t->value());
    }

    SectionBar::SectionBar(IProjectWindow *iWin, QWidget *parent) : SectionBar(*new SectionBarPrivate(), iWin, parent) {
    }

    SectionBar::~SectionBar() {
    }

    void SectionBar::initialize() {
        Q_D(SectionBar);

        auto timeline = iWin->doc()->project()->timeline();
        auto timeSigs = timeline->timeSignatures();
        auto tempos = timeline->tempos();

        connect(timeSigs, &DspxTimeSignatureListEntity::inserted, d, &SectionBarPrivate::_q_timeSignatureInserted);
        connect(timeSigs, &DspxTimeSignatureListEntity::removed, d, &SectionBarPrivate::_q_timeSignatureRemoved);

        // Add initial items
        {
            const auto &seqs = timeSigs->indexes();
            for (const auto &seq : seqs) {
                auto ts = timeSigs->at(seq);
                d->timeSigs.insert(seq, {ts, ts->barIndex()});
            }
        }

        connect(tempos, &DspxTempoListEntity::inserted, d, &SectionBarPrivate::_q_tempoInserted);
        connect(tempos, &DspxTempoListEntity::removed, d, &SectionBarPrivate::_q_tempoRemoved);

        // Add initial items
        {
            const auto &seqs = tempos->indexes();
            for (const auto &seq : seqs) {
                auto t = tempos->at(seq);
                d->tempos.insert(seq, {t, t->position()});
            }
        }

        d->timeSigsEntity = timeSigs;
        d->temposEntity = tempos;
    }

    void SectionBar::extensionInitialized() {
    }

    const QsApi::MusicTimeline *SectionBar::timeline() const {
        Q_D(const SectionBar);
        return &d->timeline;
    }

    SectionBar::SectionBar(SectionBarPrivate &d, IProjectWindow *iWin, QWidget *parent)
        : QFrame(parent), IPianoRollComponent(iWin), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

} // namespace Core
