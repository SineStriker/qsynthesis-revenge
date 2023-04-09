#ifndef CHORUSKIT_IEMGRADDON_H
#define CHORUSKIT_IEMGRADDON_H

#include <CoreApi/ActionItem.h>
#include <CoreApi/IWindowAddOn.h>

namespace IEMgr {

    namespace Internal {

        class IEMgrAddOnFactory : public Core::IWindowAddOnFactory {
        public:
            bool predicate(Core::IWindow *handle) const override;
            Core::IWindowAddOn *create(QObject *parent) override;
        };

        class IEMgrAddOn : public Core::IWindowAddOn {
            Q_OBJECT
        public:
            explicit IEMgrAddOn(QObject *parent = nullptr);
            ~IEMgrAddOn();

            void initialize() override;
            void extensionsInitialized() override;

        public:
            void reloadStrings();
            void initActions();

        private:
            Core::ActionItem *importGroupItem;
            Core::ActionItem *importItem;

            Core::ActionItem *exportGroupItem;
            Core::ActionItem *exportItem;
        };

    }

}



#endif // CHORUSKIT_IEMGRADDONFACTORY_H
