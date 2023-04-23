#ifndef CHORUSKIT_MUSICTIME_H
#define CHORUSKIT_MUSICTIME_H

#include <QList>
#include <QPair>
#include <QSharedData>

namespace ScriptMgr::Internal {

    class MusicTimePrivate;

    class MusicTime {
    public:
        MusicTime();
        ~MusicTime();

        MusicTime(double quartersPerBar, int tpqn, int measure, int beat, int tick);
        MusicTime(const QList<QPair<int, double>> &timeSignatures, int tpqn, int measure, int beat, int tick);
        MusicTime(const QList<QPair<int, double>> &timeSignatures, int tpqn, const QString &str);
        MusicTime(double quartersPerBar, int tpqn, const QString &str);

        void setTimeSignatures(const QList<QPair<int, double>> &timeSignatureList);
        QList<QPair<int, double>> timeSignatures() const;

        void setTimeSignature(int barPos, double quartersPerBar);
        void setTimeSignature(double quartersPerBar);
        double timeSignature(int barPos) const;
        int nearestTimeSignaturePosition(int barPos) const;

        void setTpqn(int tpqn);
        int tpqn() const;

        bool setMbt(int measure, int beat, int tick);
        void setTotalTick(int tick);

        int measure() const;
        int beat() const;
        int tick() const;
        int totalTick() const;

        inline int ticksTo(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return 0;
            return t.totalTick() - totalTick();
        }

        bool isValid() const;
        void fixToValid();
        bool isNull();

        QString toString() const;
        void fromString(const QString &str);

        inline bool operator!=(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return true;
            return totalTick() != t.totalTick();
        }
        inline bool operator<(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return false;
            return totalTick() < t.totalTick();
        }
        inline bool operator<=(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return false;
            return totalTick() <= t.totalTick();
        }
        inline bool operator==(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return false;
            return totalTick() == t.totalTick();
        }
        inline bool operator>(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return false;
            return totalTick() > t.totalTick();
        }
        inline bool operator>=(const MusicTime &t) const {
            if (!isValid() || !t.isValid())
                return false;
            return totalTick() >= t.totalTick();
        }

    private:
        QSharedDataPointer<MusicTimePrivate> d;
    };

} // Internal

#endif // CHORUSKIT_MUSICTIME_H
