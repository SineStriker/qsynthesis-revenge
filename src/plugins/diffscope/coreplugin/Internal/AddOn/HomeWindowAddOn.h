#ifndef HOMEWINDOWADDON_H
#define HOMEWINDOWADDON_H

#include "CoreApi/IWindowAddOn.h"

namespace Core {

    namespace Internal {

        class HomeWindowAddOnFactory : public IWindowAddOnFactory {
        public:
            bool predicate(IWindow *iWindow) const override;
            IWindowAddOn *create(QObject *parent) override;
        };

        class HomeWindowAddOn : public IWindowAddOn {
            Q_OBJECT
        public:
            explicit HomeWindowAddOn(QObject *parent = nullptr);
            ~HomeWindowAddOn();

            void initialize() override;
            void extensionsInitialized() override;

            void reloadMenuBar();
        };

    }

}

#endif // HOMEWINDOWADDON_H
