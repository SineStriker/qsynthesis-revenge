#ifndef CHORUSKIT_TRACKPANEL_H
#define CHORUSKIT_TRACKPANEL_H

#include <QFrame>

#include "PianoRoll/IDockPanel.h"

namespace Core::Internal {

    class TrackPanel : public IDockPanel {
        Q_OBJECT
    public:
        explicit TrackPanel(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~TrackPanel();

        void initialize() override;
        void extensionInitialized() override;

    protected:
        QMenu *createCardMenu() const override;
    };

}



#endif // CHORUSKIT_TRACKPANEL_H
