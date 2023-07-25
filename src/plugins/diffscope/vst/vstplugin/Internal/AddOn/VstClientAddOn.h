//
// Created by Crs_1 on 2023/7/2.
//

#ifndef CHORUSKIT_VSTCLIENTADDON_H
#define CHORUSKIT_VSTCLIENTADDON_H

#include <CoreApi/IWindowAddOn.h>

namespace Vst::Internal {

    class VstClientAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit VstClientAddOn(QObject *parent = nullptr);
        ~VstClientAddOn();

        void initialize() override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;

        void showWindow() const;
        void hideWindow() const;
        bool isWindowHidden() const;
    };

} // namespace Vst::Internal

#endif // CHORUSKIT_VSTCLIENTADDON_H
