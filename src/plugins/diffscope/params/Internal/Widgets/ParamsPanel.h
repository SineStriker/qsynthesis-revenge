#ifndef CHORUSKIT_PARAMSPANEL_H
#define CHORUSKIT_PARAMSPANEL_H

#include <QFrame>

#include <coreplugin/Interfaces/IPianoRollComponent.h>

namespace Params::Internal {

    class ParamsPanel : public QFrame, public Core::IPianoRollComponent {
        Q_OBJECT
    public:
        explicit ParamsPanel(Core::IProjectWindow *iWin, QWidget *parent = nullptr);
        ~ParamsPanel();

        void initialize() override;
        void extensionInitialized() override;
    };

}

#endif // CHORUSKIT_PARAMSPANEL_H
