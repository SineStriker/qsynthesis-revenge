//
// Created by Crs_1 on 2023/7/2.
//

#ifndef CHORUSKIT_VSTCLIENTADDON_H
#define CHORUSKIT_VSTCLIENTADDON_H

#include <CoreApi/IWindowAddOn.h>

namespace Vst::Internal {

    class VstClientAddOnFactory: public Core::IWindowAddOnFactory {
    public:
        bool predicate(Core::IWindow *handle) const override;
        Core::IWindowAddOn *create(QObject *parent) override;
    };

    class VstClientAddOn: public Core::IWindowAddOn {
        friend class VstBridge;
    public:
        explicit VstClientAddOn(QObject *parent = nullptr);

        void initialize() override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;

        void showWindow() const;
        void hideWindow() const;
    };

} // Internal

#endif // CHORUSKIT_VSTCLIENTADDON_H
