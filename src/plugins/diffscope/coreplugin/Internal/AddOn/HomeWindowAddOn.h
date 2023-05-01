#ifndef HOMEWINDOWADDON_H
#define HOMEWINDOWADDON_H

#include <QPushButton>

#include <CoreApi/ActionItem.h>

#include "CoreWindowAddOn.h"

namespace Core {

    namespace Internal {

        class HomeWindowAddOnFactory : public IWindowAddOnFactory {
        public:
            bool predicate(IWindow *handle) const override;
            IWindowAddOn *create(QObject *parent) override;
        };

        class HomeWindowAddOn : public CoreWindowAddOn {
            Q_OBJECT
        public:
            explicit HomeWindowAddOn(QObject *parent = nullptr);
            ~HomeWindowAddOn();

            void initialize() override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;

        private:
            void reloadStrings();
            void initActions();

            QAbstractButton *recentWidgetButton;

        private:
            void _q_newButtonClicked();
            void _q_openButtonClicked();
            void _q_openFileRequested(const QString &fileName);
        };

    }

}

#endif // HOMEWINDOWADDON_H
