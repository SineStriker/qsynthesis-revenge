#ifndef CHORUSKIT_MUSICTIMESIGNATURE_H
#define CHORUSKIT_MUSICTIMESIGNATURE_H

#include <QDebug>
#include <QString>
namespace ScriptMgr::Internal {

    class MusicTimeSignature {
    public:
        MusicTimeSignature(int numerator = 4, int denominator = 4);

        inline int numerator() const {
            return m_numerator;
        }

        inline int denominator() const {
            return m_denominator;
        }

        bool isValid() const;

        int ticksPerBar(int tpqn) const;
        int ticksPerBeat(int tpqn) const;

        QString toString() const;

        inline bool operator==(const MusicTimeSignature &t) const {
            return m_numerator == t.m_numerator && m_denominator == t.m_denominator;
        }
        inline bool operator!=(const MusicTimeSignature &t) const {
            return m_numerator != t.m_numerator || m_denominator != t.m_denominator;
        }
        friend QDebug operator<<(QDebug debug, const MusicTimeSignature &t);
    private:
        int m_numerator;
        int m_denominator;
    };

} // Internal

#endif // CHORUSKIT_MUSICTIMESIGNATURE_H
