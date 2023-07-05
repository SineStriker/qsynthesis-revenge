#ifndef CHORUSKIT_PARAMSADDON_H
#define CHORUSKIT_PARAMSADDON_H

#include <QAbstractButton>

#include <CoreApi/IWindow.h>
#include <CoreApi/IWindowAddOn.h>

#include <Internal/Widgets/ParamsPanel.h>

#include "ParamsGlobal.h"

namespace Params {

    namespace Internal {

        class ParamsAddOn : public Core::IWindowAddOn {
            Q_OBJECT
        public:
            Q_INVOKABLE explicit ParamsAddOn(QObject *parent = nullptr);
            ~ParamsAddOn();

            void initialize() override;
            void extensionsInitialized() override;

        private:
            void reloadStrings();
            void initActions();

        private:
            Core::ActionItem *paramsPanelVisibleItem;

            // Panels
            ParamsPanel *paramsPanel;
            QAbstractButton *paramsButton;
        };

    }

}


#endif // CHORUSKIT_PARAMSADDON_H
