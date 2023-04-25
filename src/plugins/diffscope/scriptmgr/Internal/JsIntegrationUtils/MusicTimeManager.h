#ifndef CHORUSKIT_MUSICTIMEMANAGER_H
#define CHORUSKIT_MUSICTIMEMANAGER_H

#include "MusicTime.h"
#include "MusicTimeSignature.h"
#include <QMap>
#include <QScopedPointer>
namespace ScriptMgr::Internal {

    class MusicTimeManagerPrivate;

    class MusicTimeManager: public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeManager)
    public:
        MusicTimeManager(int tpqn = 480);
        ~MusicTimeManager();

        void setTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList);
        void removeTimeSignatures(const QList<int> &barPosList);
        QList<QPair<int, MusicTimeSignature>> timeSignatures() const;
        void setTimeSignature(int barPos, MusicTimeSignature timeSignature);
        MusicTimeSignature timeSignature(int barPos) const;
        int nearestTimeSignaturePosition(int barPos) const;
        void removeTimeSignature(int barPos);

        void setTempos(const QList<QPair<int, double>> &tempoList);
        void removeTempos(const QList<int> &tickPosList);
        QList<QPair<int, double>> tempos() const;
        void setTempo(int tickPos, double tempo);
        double tempo(int tickPos) const;
        int nearestTempoPosition(int tickPos) const;
        void removeTempo(int tickPos);

        MusicTime create();
        MusicTime create(int measure, int beat, int tick);
        MusicTime create(const QString &str);
        MusicTime create(double msec);

        void thisIsATest();
    signals:
        void timeSignatureChanged();
        void tempoChanged();
        void changed();
    private:
        friend class MusicTime;
        QScopedPointer<MusicTimeManagerPrivate> d_ptr;
        explicit MusicTimeManager(MusicTimeManagerPrivate &d, int tpqn);
    };

} // Internal

#endif // CHORUSKIT_MUSICTIMEMANAGER_H
