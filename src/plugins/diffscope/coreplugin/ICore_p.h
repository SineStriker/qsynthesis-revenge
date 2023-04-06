#ifndef ICORE_P_H
#define ICORE_P_H

#include "ICore.h"
#include "Preferences/PreferenceRegistry.h"

namespace Core {

    class ICorePrivate {
        Q_DECLARE_PUBLIC(ICore)
    public:
        ICorePrivate();
        void init();

        ICore *q_ptr;

        ActionSystem *actionSystem;
        WindowSystem *windowSystem;
        DialogHelper *dialogHelper;
        PreferenceRegistry *preferenceRegistry;
        SettingCatalog *settingCatalog;
    };

}

#endif // ICORE_P_H
