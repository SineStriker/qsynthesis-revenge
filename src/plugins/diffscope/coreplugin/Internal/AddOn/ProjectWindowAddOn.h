#ifndef PROJECTWINDOWADDON_H
#define PROJECTWINDOWADDON_H

#include "CDockCard.h"

#include "CoreApi/ActionItem.h"

#include "CoreWindowAddOn.h"
#include "Internal/Widgets/PhonemePanel.h"
#include "Internal/Widgets/TrackPanel.h"

namespace Core {

    namespace Internal {

        class ProjectWindowAddOn : public CoreWindowAddOn {
            Q_OBJECT
        public:
            Q_INVOKABLE explicit ProjectWindowAddOn(QObject *parent = nullptr);
            ~ProjectWindowAddOn();

            void initialize() override;
            void extensionsInitialized() override;

        private:
            void reloadStrings();
            void initActions();

        protected:
            ActionItem *editItem;
            ActionItem *viewItem;

            // File
            ActionItem *saveGroupItem;
            ActionItem *saveFileItem;
            ActionItem *saveAsFileItem;

            // Edit
            ActionItem *undoGroupItem;
            ActionItem *undoItem;
            ActionItem *redoItem;

            ActionItem *editGroupItem;
            ActionItem *cutItem;
            ActionItem *copyItem;
            ActionItem *pasteItem;
            ActionItem *deleteItem;

            ActionItem *selectGroupItem;
            ActionItem *selectAllItem;
            ActionItem *deselectItem;

            // View
            ActionItem *appearanceMenuItem;
            ActionItem *mainMenuVisibleItem;
            ActionItem *mainToolbarVisibleItem;
            ActionItem *dockVisibleItem;
            ActionItem *statusBarVisibleItem;

            ActionItem *dockPanelsVisibilityMenuItem;
            ActionItem *trackPanelVisibleItem;

            ActionItem *floatingPanelsVisibilityMenuItem;
            ActionItem *phonemePanelVisibleItem;

            // Main Toolbar
            ActionItem *timerGroupItem;
            ActionItem *timerLabelItem;

            ActionItem *quantizeGroupItem;
            ActionItem *quantizeSelectorItem;

            ActionItem *playControlGroupItem;
            ActionItem *playItem;
            ActionItem *stopItem;
            ActionItem *moveToStartItem;
            ActionItem *moveToEndItem;

            ActionItem *playAssistGroupItem;
            ActionItem *metronomeItem;
            ActionItem *loopPlayItem;

            ActionItem *mainToolbarStretchItem;
            ActionItem *mainToolbarHelpGroupItem;

            // Invisible
            ActionItem *selectPianoKeyWidgetItem;

            // Panels
            TrackPanel *trackPanel;
            CDockCard *trackButton;

            PhonemePanel *phonemePanel;
            QAbstractButton *phonemeButton;
        };

    } // namespace Internal

} // namespace Core

#endif // PROJECTWINDOWADDON_H
