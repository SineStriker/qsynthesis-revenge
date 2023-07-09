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

        int currentWidth() const;
        void setCurrentWidth(int currentWidth);

        int currentHeight() const;
        void setCurrentHeight(int currentHeight);

        int currentSnap() const;
        void setCurrentSnap(int currentSnap);

    signals:
        void timeSignatureAdded(int bar, const QsApi::MusicTimeSignature &timeSignature);
        void timeSignatureRemoved(int bar);

        void tempoAdded(int tick, double tempo);
        void tempoRemoved(int tick);

        void currentWidthChanged(int w);
        void currentHeightChanged(int h);
        void currentSnapChanged(int s);

        void changed();

    private:
        MusicTimeManagerPrivate *d;
    };

}

#endif // CHORUSKIT_MUSICTIMEMANAGER_H
