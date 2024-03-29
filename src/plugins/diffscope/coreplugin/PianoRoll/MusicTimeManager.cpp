#include "MusicTimeManager_p.h"
#include "MusicTimeManager.h"


namespace Core {

    MusicTimeManagerPrivate::MusicTimeManagerPrivate(Core::MusicTimeManager *q) : q(q) {
        timeSigsEntity = nullptr;
        temposEntity = nullptr;

        m_currentWidth = 100;
        m_currentHeight = 30;
        m_currentSnap = 240;
    }

    MusicTimeManagerPrivate::~MusicTimeManagerPrivate() {
    }

    MusicTimeManager::MusicTimeManager(IProjectWindow *iWin, QObject *parent)
        : QObject(parent), IPianoRollComponent(iWin), d(new MusicTimeManagerPrivate(this)) {
    }

    MusicTimeManager::~MusicTimeManager() {
        delete d;
    }

    void MusicTimeManager::initialize() {
        auto timeline = iWin->doc()->project()->timeline();
        auto timeSigs = timeline->timeSignatures();
        auto tempos = timeline->tempos();

        connect(timeSigs, &DspxTimeSignatureListEntity::inserted, d,
                &MusicTimeManagerPrivate::_q_timeSignatureInserted);
        connect(timeSigs, &DspxTimeSignatureListEntity::removed, d, &MusicTimeManagerPrivate::_q_timeSignatureRemoved);

        // Add initial items
        {
            const auto &seqs = timeSigs->indexes();
            for (const auto &seq : seqs) {
                auto ts = timeSigs->at(seq);
                d->timeSigs.insert(seq, {ts, ts->barIndex()});
                d->m_timeline.addTimeSignature(ts->barIndex(), ts->value());
            }
        }

        connect(tempos, &DspxTempoListEntity::inserted, d, &MusicTimeManagerPrivate::_q_tempoInserted);
        connect(tempos, &DspxTempoListEntity::removed, d, &MusicTimeManagerPrivate::_q_tempoRemoved);

        // Add initial items
        {
            const auto &seqs = tempos->indexes();
            for (const auto &seq : seqs) {
                auto t = tempos->at(seq);
                d->tempos.insert(seq, {t, t->position()});
                d->m_timeline.addTempo(t->position(), t->value());
            }
        }

        d->timeSigsEntity = timeSigs;
        d->temposEntity = tempos;
    }

    void MusicTimeManager::extensionInitialized() {
    }

    const QsApi::MusicTimeline *MusicTimeManager::timeline() const {
        return &d->m_timeline;
    }

    int MusicTimeManager::currentWidth() const {
        return d->m_currentWidth;
    }

    void MusicTimeManager::setCurrentWidth(int currentWidth) {
        d->m_currentWidth = currentWidth;
        emit currentWidthChanged(currentWidth);
    }

    int MusicTimeManager::currentHeight() const {
        return d->m_currentHeight;
    }

    void MusicTimeManager::setCurrentHeight(int currentHeight) {
        d->m_currentHeight = currentHeight;
        emit currentHeightChanged(currentHeight);
    }

    int MusicTimeManager::currentSnap() const {
        return d->m_currentSnap;
    }

    void MusicTimeManager::setCurrentSnap(int currentSnap) {
        d->m_currentSnap = currentSnap;
        emit currentHeightChanged(currentSnap);
    }

    void MusicTimeManagerPrivate::_q_timeSignatureInserted(int seq, DspxTimeSignatureEntity *ts) {
        timeSigs.insert(seq, {ts, ts->barIndex()});
        m_timeline.addTimeSignature(ts->barIndex(), ts->value());

        connect(ts, &DspxTimeSignatureEntity::barIndexChanged, this,
                &MusicTimeManagerPrivate::_q_timeSignatureIndexChanged);
        connect(ts, &DspxTimeSignatureEntity::valueChanged, this,
                &MusicTimeManagerPrivate::_q_timeSignatureValueChanged);

        emit q->timeSignatureAdded(ts->barIndex(), ts->value());
        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_timeSignatureRemoved(int seq) {
        auto it = timeSigs.find(seq);
        auto &ts = it.value().ts;

        disconnect(ts, &DspxTimeSignatureEntity::barIndexChanged, this,
                   &MusicTimeManagerPrivate::_q_timeSignatureIndexChanged);
        disconnect(ts, &DspxTimeSignatureEntity::valueChanged, this,
                   &MusicTimeManagerPrivate::_q_timeSignatureValueChanged);

        timeSigs.erase(it);

        emit q->timeSignatureRemoved(ts->barIndex());
        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_timeSignatureIndexChanged(int bar) {
        auto entity = static_cast<DspxTimeSignatureEntity *>(sender());
        auto seq = timeSigsEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto ts = timeSigs.value(seq);

        // Remove old time signature
        m_timeline.removeTimeSignature(ts.bar);
        emit q->timeSignatureRemoved(ts.bar);

        ts.bar = bar;

        // Insert new time signature
        m_timeline.addTimeSignature(bar, ts.ts->value());
        emit q->timeSignatureAdded(bar, ts.ts->value());

        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_timeSignatureValueChanged(int num, int den) {
        Q_UNUSED(num)
        Q_UNUSED(den)

        auto entity = static_cast<DspxTimeSignatureEntity *>(sender());
        auto seq = timeSigsEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto ts = timeSigs.value(seq);

        // Replace time signature
        m_timeline.addTimeSignature(ts.bar, ts.ts->value());
        emit q->timeSignatureAdded(ts.bar, ts.ts->value());

        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_tempoInserted(int seq, DspxTempoEntity *t) {
        tempos.insert(seq, {t, t->position()});
        m_timeline.addTempo(t->position(), t->value());

        connect(t, &DspxTempoEntity::positionChanged, this, &MusicTimeManagerPrivate::_q_tempoPosChanged);
        connect(t, &DspxTempoEntity::valueChanged, this, &MusicTimeManagerPrivate::_q_tempoValueChanged);

        emit q->tempoAdded(t->position(), t->value());
        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_tempoRemoved(int seq) {
        auto it = tempos.find(seq);
        auto &t = it.value().t;

        disconnect(t, &DspxTempoEntity::positionChanged, this, &MusicTimeManagerPrivate::_q_tempoPosChanged);
        disconnect(t, &DspxTempoEntity::valueChanged, this, &MusicTimeManagerPrivate::_q_tempoValueChanged);

        tempos.erase(it);

        emit q->tempoRemoved(t->position());
        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_tempoPosChanged(int pos) {
        auto entity = static_cast<DspxTempoEntity *>(sender());
        auto seq = temposEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto t = tempos.value(seq);

        // Remove old tempo
        m_timeline.removeTempo(t.pos);
        emit q->tempoRemoved(t.pos);

        t.pos = pos;

        // Insert new tempo
        m_timeline.addTempo(pos, t.t->value());
        emit q->tempoAdded(pos, t.t->value());

        emit q->changed();
    }

    void MusicTimeManagerPrivate::_q_tempoValueChanged(double val) {
        auto entity = static_cast<DspxTempoEntity *>(sender());
        auto seq = temposEntity->treeItem()->recordSequenceOf(entity->treeItem());

        auto t = tempos.value(seq);

        // Replace tempo
        m_timeline.addTempo(t.pos, t.t->value());
        emit q->tempoAdded(t.pos, t.t->value());

        emit q->changed();
    }

}