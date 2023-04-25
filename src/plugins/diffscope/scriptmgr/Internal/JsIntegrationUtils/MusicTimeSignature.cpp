#include "MusicTimeSignature.h"
#include <QRegularExpression>

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
} // Internal