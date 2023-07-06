#ifndef CHORUSKIT_PHONEMEPANEL_H
#define CHORUSKIT_PHONEMEPANEL_H

#include <QFrame>

#include "Interfaces/IPianoRollComponent.h"

namespace Core::Internal {

    class PhonemePanel : public QFrame, public IPianoRollComponent {
        Q_OBJECT
    public:
        explicit PhonemePanel(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~PhonemePanel();

        void initialize() override;
        void extensionInitialized() override;
    };

}



#endif // CHORUSKIT_PHONEMEPANEL_H
