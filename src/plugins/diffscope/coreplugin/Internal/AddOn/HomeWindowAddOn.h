#ifndef HOMEWINDOWADDON_H
#define HOMEWINDOWADDON_H

#include "CoreApi/ActionItem.h"
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

            void reloadStrings();
            void reloadMenuBar();

        private:
            ActionItem *fileItem;
            ActionItem *editItem;
            ActionItem *helpItem;

            ActionItem *openGroupItem;
            ActionItem *newFileItem;
            ActionItem *openFileItem;

            ActionItem *preferenceGroupItem;
            ActionItem *settingsItem;

            ActionItem *aboutGroupItem;
            ActionItem *aboutAppItem;
            ActionItem *aboutQtItem;
        };

    }

}

#endif // HOMEWINDOWADDON_H
