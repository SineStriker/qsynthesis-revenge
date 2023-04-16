//
// Created by Crs_1 on 2023/4/16.
//

#ifndef CHORUSKIT_BATPROCADDON_H
#define CHORUSKIT_BATPROCADDON_H

#include "CoreApi/IWindowAddOn.h"

namespace BatProc::Internal {

    class BatProcAddOnFactory : public Core::IWindowAddOnFactory {
    public:
        bool predicate(Core::IWindow *handle) const override;
        Core::IWindowAddOn *create(QObject *parent) override;
    };

    class BatProcAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit BatProcAddOn(QObject *parent = nullptr);
        ~BatProcAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();
        void loadBuiltInScripts();
        void loadUserScripts();
    private:


    };

} // Internal

#endif // CHORUSKIT_BATPROCADDON_H
