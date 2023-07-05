#ifndef HOMEWINDOWADDON_H
#define HOMEWINDOWADDON_H

#include <QPushButton>

#include "CoreWindowAddOn.h"

namespace Core {

    namespace Internal {

        class HomeWindowAddOn : public CoreWindowAddOn {
            Q_OBJECT
        public:
            Q_INVOKABLE explicit HomeWindowAddOn(QObject *parent = nullptr);
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

    } // namespace Internal

} // namespace Core

#endif // HOMEWINDOWADDON_H
