#include "MusicTimeSignature.h"

namespace ScriptMgr::Internal {
    MusicTimeSignature::MusicTimeSignature(int numerator, int denominator): m_numerator(numerator), m_denominator(denominator) {
    }
    bool MusicTimeSignature::isValid() const {
        if(m_numerator <= 0) return false;
        if(m_denominator != 2 && m_denominator != 4 && m_denominator != 8 && m_denominator != 16) return false;
        return true;
    }
    int MusicTimeSignature::ticksPerBar(int tpqn) const {
        return tpqn * m_numerator * 4 / m_denominator;
    }
    int MusicTimeSignature::ticksPerBeat(int tpqn) const {
        return tpqn * 4 / m_denominator;
    }
    QString MusicTimeSignature::toString() const {
        return QString("%1/%2").arg(QString::number(m_numerator), QString::number(m_denominator));
    }
    QDebug operator<<(QDebug debug, const MusicTimeSignature &t) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "MusicTimeSignature(" << t.m_numerator << "/" << t.m_denominator << ")";
        return debug;
    }
} // Internal