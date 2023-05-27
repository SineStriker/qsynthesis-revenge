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
            ActionItem *saveGroupItem;
            ActionItem *saveFileItem;
            ActionItem *saveAsFileItem;

            ActionItem *exitGroupItem;
            ActionItem *closeFileItem;

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
        };

    }

}

#endif // PROJECTWINDOWADDON_H
