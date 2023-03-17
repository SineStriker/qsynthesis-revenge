#ifndef ICOREWINDOWADDON_H
#define ICOREWINDOWADDON_H

#include "Window/IWindowAddOn.h"

namespace Core {

    namespace Internal {

        class ICoreWindowAddOnFactory : public IWindowAddOnFactory {
        public:
            bool predicate(IWindow *iWindow) const override;
            IWindowAddOn *create(QObject *parent) override;
        };

        class ICoreWindowAddOn : public IWindowAddOn {
            Q_OBJECT
        public:
            explicit ICoreWindowAddOn(QObject *parent = nullptr);
            ~ICoreWindowAddOn();

            void initialize() override;
            void extensionsInitialized() override;
        };

    }

}



#endif // ICOREWINDOWADDON_H
