#ifndef CHORUSKIT_MUSICTIMEMANAGER_H
#define CHORUSKIT_MUSICTIMEMANAGER_H

#include <MusicUtil/MusicTimeline.h>

#include "Interfaces/IPianoRollComponent.h"

namespace Core {

    class IProjectWindow;

    class MusicTimeManagerPrivate;

    class MusicTimeManager : public QObject, public IPianoRollComponent {
        Q_OBJECT
    public:
        explicit MusicTimeManager(IProjectWindow *iWin, QObject *parent = nullptr);
        ~MusicTimeManager();

    public:
        void initialize() override;
        void extensionInitialized() override;

        const QsApi::MusicTimeline *timeline() const;

    signals:
        void timeSignatureAdded(int bar, QsApi::MusicTimeSignature timeSignature);
        void timeSignatureRemoved(int bar);
        void timeSignatureMoved(int bar, int oldBar);

        void tempoAdded(int tick, double tempo);
        void tempoRemoved(int tick, double tempo);
        void tempoMoved(int tick, int oldTick);

    private:
        MusicTimeManagerPrivate *d;
    };

}

#endif // CHORUSKIT_MUSICTIMEMANAGER_H
