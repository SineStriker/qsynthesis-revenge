#ifndef CHORUSKIT_TMPLADDON_H
#define CHORUSKIT_TMPLADDON_H

#include <QAbstractButton>

#include <CoreApi/ActionItem.h>
#include <CoreApi/IWindowAddOn.h>

namespace TemplatePlg::Internal {
        class TmplAddOnFactory : public Core::IWindowAddOnFactory {
        public:
            bool predicate(Core::IWindow *handle) const override;
            Core::IWindowAddOn *create(QObject *parent) override;
        };

        class TmplAddOn : public Core::IWindowAddOn {
            Q_OBJECT
        public:
            explicit TmplAddOn(QObject *parent = nullptr);
            ~TmplAddOn() override;

            void initialize() override;
            void extensionsInitialized() override;

        public:
            void reloadStrings();
            void initActions();

        private slots:
            static void _q_tmplButtonClicked();

        private:
            Core::ActionItem *templateGroupItem;
            Core::ActionItem *tmplMenu;
            Core::ActionItem *tmplImpItem;
        };

    }



#endif // CHORUSKIT_TMPLADDON_H
