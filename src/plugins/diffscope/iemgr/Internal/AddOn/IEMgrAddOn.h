#ifndef CHORUSKIT_IEMGRADDON_H
#define CHORUSKIT_IEMGRADDON_H

#include <QAbstractButton>

#include <CoreApi/ActionItem.h>
#include <CoreApi/IWindowAddOn.h>

namespace IEMgr {

    namespace Internal {

        class IEMgrAddOn : public Core::IWindowAddOn {
            Q_OBJECT
        public:
            Q_INVOKABLE explicit IEMgrAddOn(QObject *parent = nullptr);
            ~IEMgrAddOn();

            void initialize() override;
            void extensionsInitialized() override;

        public:
            void reloadStrings();
            void initActions();

            void initImportButton();

        private:
            Core::ActionItem *importExportGroupItem;

            Core::ActionItem *importItem;
            Core::ActionItem *importProjectItem;
            Core::ActionItem *importAudioItem;

            Core::ActionItem *exportItem;
            Core::ActionItem *exportProjectItem;
            Core::ActionItem *exportAudioItem;

            QAbstractButton *importButton;

            void _q_importButtonClicked();
        };

    } // namespace Internal

} // namespace IEMgr



#endif // CHORUSKIT_IEMGRADDONFACTORY_H
