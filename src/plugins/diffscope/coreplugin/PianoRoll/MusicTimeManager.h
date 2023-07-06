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

        QsApi::MusicTimeline *timeline() const;

    private:
        MusicTimeManagerPrivate *d;
    };

}

#endif // CHORUSKIT_MUSICTIMEMANAGER_H
