#ifndef CHORUSKIT_COREWINDOWADDON_H
#define CHORUSKIT_COREWINDOWADDON_H

#include <Collections/QMChronSet.h>

#include <CoreApi/ActionItem.h>
#include <CoreApi/DocumentSpec.h>
#include <CoreApi/IWindowAddOn.h>

namespace Core {

    namespace Internal {

        class CoreWindowAddOn : public IWindowAddOn {
            Q_OBJECT
        public:
            explicit CoreWindowAddOn(QObject *parent = nullptr);
            ~CoreWindowAddOn();

            void initialize() override;
            void extensionsInitialized() override;

        private:
            void reloadStrings();
            void initActions();

        protected:
            ActionItem *fileItem;
            ActionItem *helpItem;

            ActionItem *openGroupItem;
            ActionItem *newFileItem;
            ActionItem *openFileItem;
            ActionItem *openRecentItem;

            ActionItem *preferenceMenuItem;
            ActionItem *settingsItem;
            ActionItem *colorThemesItem;
            ActionItem *languagesItem;

            ActionItem *aboutGroupItem;
            ActionItem *aboutPluginsItem;
            ActionItem *aboutAppItem;
            ActionItem *aboutQtItem;

            ActionItem *welcomeGroupItem;
            ActionItem *showHomeItem;
            ActionItem *findActionItem;

        protected slots:
            void reloadRecentMenu();

        private:
            void _q_recentFilesChanged();
        };

    }
}


#endif // CHORUSKIT_COREWINDOWADDON_H
