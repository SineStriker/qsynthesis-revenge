#ifndef CHORUSKIT_MUSICTIME_H
#define CHORUSKIT_MUSICTIME_H

#include <QList>
#include <QPair>

namespace ScriptMgr::Internal {

    class MusicTimeManager;

    class MusicTime {
    public:
        ~MusicTime();
        bool setMbt(int measure, int beat, int tick);

        int measure() const;
        int beat() const;
        int tick() const;
        std::tuple<int, int, int> mbt() const;
        inline int totalTick() const {
            return m_tick;
        }

        QString toString() const;
        bool fromString(const QString &str);

        double msec() const;
        bool fromMsec(double msec);

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
        inline MusicTime &operator+=(int val) {
            m_tick += val;
            return *this;
        }
        inline MusicTime &operator-=(int val) {
            m_tick -= val;
            return *this;
        }
        inline MusicTime operator+(int val) const {
            MusicTime t(*this);
            t.m_tick += val;
            return t;
        }
        friend inline MusicTime operator+(int val, const MusicTime &t) {
            MusicTime t2(t);
            t2.m_tick += val;
            return t2;
        }
        inline MusicTime operator-(int val) const {
            MusicTime t(*this);
            t.m_tick -= val;
            return t;
        }
        friend inline MusicTime operator-(int val, const MusicTime &t) {
            MusicTime t2(t);
            t2.m_tick -= val;
            return t2;
        }
    private:
        friend class MusicTimeManager;
        const MusicTimeManager *m_manager;
        int m_tick = 0;
    protected:
        explicit MusicTime(MusicTimeManager *manager);
        MusicTime(MusicTimeManager *manager, int measure, int beat, int tick);
        MusicTime(MusicTimeManager *manager, const QString &str);
        MusicTime(MusicTimeManager *manager, double msec);
    };

} // Internal

#endif // CHORUSKIT_MUSICTIME_H
