#ifndef CHORUSKIT_MUSICTIMEMANAGER_H
#define CHORUSKIT_MUSICTIMEMANAGER_H

#include "MusicTime.h"
#include "MusicTimeSignature.h"
#include <QMap>
#include <QScopedPointer>
namespace ScriptMgr::Internal {

    typedef int measure_t;
    typedef int beat_t;
    typedef int tick_t;
    typedef double msec_t;
    typedef double tempo_t;

    class MusicTimeManagerPrivate;

    class MusicTimeManager: public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeManager)
    public:
        MusicTimeManager(tick_t tpqn = 480);
        ~MusicTimeManager();

        void setTimeSignatures(const QList<QPair<measure_t, MusicTimeSignature>> &timeSignatureList);
        void removeTimeSignatures(const QList<measure_t > &barPosList);
        QList<QPair<measure_t, MusicTimeSignature>> timeSignatures() const;
        void setTimeSignature(measure_t barPos, MusicTimeSignature timeSignature);
        MusicTimeSignature timeSignature(measure_t barPos) const;
        measure_t nearestTimeSignaturePosition(measure_t barPos) const;
        void removeTimeSignature(measure_t barPos);

        void setTempos(const QList<QPair<tick_t, tempo_t>> &tempoList);
        void removeTempos(const QList<tick_t> &tickPosList);
        QList<QPair<tick_t, tempo_t>> tempos() const;
        void setTempo(tick_t tickPos, tempo_t tempo);
        tempo_t tempo(tick_t tickPos) const;
        tick_t nearestTempoPosition(tick_t tickPos) const;
        void removeTempo(tick_t tickPos);

        MusicTime create();
        MusicTime create(measure_t measure, beat_t beat, tick_t tick);
        MusicTime create(const QString &str);
        MusicTime create(msec_t msec);

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
