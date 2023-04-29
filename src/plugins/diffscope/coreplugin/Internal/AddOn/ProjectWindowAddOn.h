#ifndef PROJECTWINDOWADDON_H
#define PROJECTWINDOWADDON_H

#include "CoreApi/ActionItem.h"

#include "CoreWindowAddOn.h"

namespace Core {

    namespace Internal {

        class ProjectWindowAddOnFactory : public IWindowAddOnFactory {
        public:
            bool predicate(IWindow *handle) const override;
            IWindowAddOn *create(QObject *parent) override;
        };

        class ProjectWindowAddOn : public CoreWindowAddOn {
            Q_OBJECT
        public:
            explicit ProjectWindowAddOn(QObject *parent = nullptr);
            ~ProjectWindowAddOn();

            void initialize() override;
            void extensionsInitialized() override;

        private:
            void reloadStrings();
            void initActions();

        protected:
            ActionItem *welcomeGroupItem;
            ActionItem *showHomeItem;
        };

    }

}

#endif // PROJECTWINDOWADDON_H
