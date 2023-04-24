#ifndef CHORUSKIT_MUSICTIME_H
#define CHORUSKIT_MUSICTIME_H

#include <QList>
#include <QPair>

namespace ScriptMgr::Internal {

    class MusicTime {
    public:
        ~MusicTime();
        void setMbt(int measure, int beat, int tick);

        int measure() const;
        int beat() const;
        int tick() const;
        inline int totalTick() const {
            return m_tick;
        }

        inline bool isNull() const {
            return m_tick == -1;
        };

        QString toString() const;
        void fromString(const QString &str);

        double msec() const;

        inline bool operator!=(const MusicTime &t) const {
            return m_tick != t.m_tick;
        }
        inline bool operator<(const MusicTime &t) const {
            return m_tick < t.m_tick;
        }
        inline bool operator<=(const MusicTime &t) const {
            return m_tick <= t.m_tick;
        }
        inline bool operator==(const MusicTime &t) const {
            return m_tick == t.m_tick;
        }
        inline bool operator>(const MusicTime &t) const {
            return m_tick > t.m_tick;
        }
        inline bool operator>=(const MusicTime &t) const {
            return m_tick >= t.m_tick;
        }
    private:
        friend class MusicTimeManager;
        MusicTimeManager *m_manager;
        int m_tick = -1;
    protected:
        explicit MusicTime(MusicTimeManager *manager);
        MusicTime(MusicTimeManager *manager, int measure, int beat, int tick);
        MusicTime(MusicTimeManager *manager, const QString &str);
    };

} // Internal

#endif // CHORUSKIT_MUSICTIME_H
