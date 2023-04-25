#include "MusicTime.h"
#include <QRegularExpression>
#include <QTextStream>

#include "MusicTimeManager_p.h"

#define MGR_D const MusicTimeManagerPrivate * mgr_d = m_manager->d_func()

namespace ScriptMgr::Internal {

    MusicTime::~MusicTime() {
    }
    bool MusicTime::setMbt(int measure, int beat, int tick) {
        MGR_D;
        if(measure < 0 || beat < 0 || tick < 0) return false;
        auto timeSig = m_manager->timeSignature(measure);
        auto refMeasure = m_manager->nearestTimeSignaturePosition(measure);
        auto refTick = mgr_d->revMeasureMap[refMeasure];
        tick += refTick + (measure - refMeasure) * timeSig.ticksPerBar(mgr_d->tpqn);
        tick += beat * timeSig.ticksPerBeat(mgr_d->tpqn);
        m_tick = tick;
        return true;
    }
    int MusicTime::measure() const {
        MGR_D;
        int refTick = mgr_d->findNearestTickWithTimeSignature(m_tick);
        int refMeasure = mgr_d->measureMap[refTick];
        MusicTimeSignature timeSig = mgr_d->timeSignatureMap[refMeasure];
        return refMeasure + (m_tick - refTick) / timeSig.ticksPerBar(mgr_d->tpqn);
    }
    int MusicTime::beat() const {
        MGR_D;
        int refTick = mgr_d->findNearestTickWithTimeSignature(m_tick);
        int refMeasure = mgr_d->measureMap[refTick];
        MusicTimeSignature timeSig = mgr_d->timeSignatureMap[refMeasure];
        return ((m_tick - refTick) % timeSig.ticksPerBar(mgr_d->tpqn)) / timeSig.ticksPerBeat(mgr_d->tpqn);
    }
    int MusicTime::tick() const {
        MGR_D;
        int refTick = mgr_d->findNearestTickWithTimeSignature(m_tick);
        int refMeasure = mgr_d->measureMap[refTick];
        MusicTimeSignature timeSig = mgr_d->timeSignatureMap[refMeasure];
        return ((m_tick - refTick) % timeSig.ticksPerBar(mgr_d->tpqn)) % timeSig.ticksPerBeat(mgr_d->tpqn);
    }
    std::tuple<int, int, int> MusicTime::mbt() const {
        MGR_D;
        int refTick = mgr_d->findNearestTickWithTimeSignature(m_tick);
        int refMeasure = mgr_d->measureMap[refTick];
        MusicTimeSignature timeSig = mgr_d->timeSignatureMap[refMeasure];
        return {
            refMeasure + (m_tick - refTick) / timeSig.ticksPerBar(mgr_d->tpqn),
            ((m_tick - refTick) % timeSig.ticksPerBar(mgr_d->tpqn)) / timeSig.ticksPerBeat(mgr_d->tpqn),
            ((m_tick - refTick) % timeSig.ticksPerBar(mgr_d->tpqn)) % timeSig.ticksPerBeat(mgr_d->tpqn),
        };
    }

    QString MusicTime::toString() const {
        QString str;
        QTextStream textStream(&str);
        auto t = mbt();
        textStream << std::get<0>(t) + 1 << ":" << std::get<1>(t) + 1 << ":";
        textStream.setFieldWidth(3);
        textStream.setFieldAlignment(QTextStream::AlignRight);
        textStream.setPadChar('0');
        textStream << std::get<2>(t);
        return str;
    }
    bool MusicTime::fromString(const QString &str) {
        QRegularExpression rx(R"(^\s*(\d+)\s*[:\xff1a]?\s*(\d*)\s*[:\xff1a]?\s*(\d*)\s*$)");
        auto match = rx.match(str);
        if(!match.hasMatch()) return false;
        setMbt(match.captured(1).toInt() - 1, match.captured(2).toInt() - 1, match.captured(3).toInt());
        return true;
    }
    double MusicTime::msec() const {
        MGR_D;
        auto refTick = m_manager->nearestTempoPosition(m_tick);
        auto tempo = mgr_d->tempoMap[refTick];
        auto refMsec = mgr_d->msecSumMap[refTick];
        return refMsec + (m_tick - refTick) * (60.0 * 1000.0) / (mgr_d->tpqn * tempo);
    }
    bool MusicTime::fromMsec(double msec) {
        MGR_D;
        if(msec < 0) return false;
        auto refMsec = mgr_d->findNearestMsecWithTempo(msec);
        auto refTick = mgr_d->revMsecSumMap[refMsec];
        auto tempo = mgr_d->tempoMap[refTick];
        auto deltaTick = (int)std::round((msec - refMsec) / (60.0 * 1000.0) * (mgr_d->tpqn * tempo));
        m_tick = refTick + deltaTick;
        return true;
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager): m_manager(manager) {
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, int measure, int beat, int tick): MusicTime(manager) {
        setMbt(measure, beat, tick);
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, const QString &str): MusicTime(manager) {
        fromString(str);
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, double msec): MusicTime(manager) {
        fromMsec(msec);
    }

} // Internal