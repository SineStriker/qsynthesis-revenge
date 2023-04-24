#ifndef CHORUSKIT_MUSICTIMEMANAGER_H
#define CHORUSKIT_MUSICTIMEMANAGER_H

#include "MusicTime.h"
#include <QMap>
#include <QSharedPointer>
namespace ScriptMgr::Internal {

    class MusicTimeManagerPrivate;

    struct MusicTimeSignature {
        int numerator;
        int denominator;
    };

    class MusicTimeManager {
    public:
        MusicTimeManager();

        void setTpqn(int tpqn);
        int tpqn() const;

        void setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList);
        QList<QPair<int, MusicTimeSignature>> timeSignatures() const;
        void setTimeSignature(int barPos, MusicTimeSignature timeSignature);
        int timeSignature(int barPos) const;
        int nearestTimeSignaturePosition(int barPos) const;
        void removeTimeSiganture(int barPos);

        void setTempos(const QList<QPair<int, double>> &tempoList);
        QList<QPair<int, double>> tempos() const;
        void setTempo(int tickPos, double tempo);
        double tempo(int tickPos) const;
        int nearestTempoPosition(int tickPos) const;
        void removeTempo(int tickPos);

        MusicTime create();
        MusicTime create(int measure, int beat, int tick);
        MusicTime create(const QString &str);
    private:
        Q_DECLARE_PRIVATE(MusicTimeManager)
        friend class MusicTime;
        QScopedPointer<MusicTimeManagerPrivate> d_ptr;
        explicit MusicTimeManager(MusicTimeManagerPrivate &d);
    };

} // Internal

#endif // CHORUSKIT_MUSICTIMEMANAGER_H
