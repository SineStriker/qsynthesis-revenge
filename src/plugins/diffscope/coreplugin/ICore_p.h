#ifndef ICORE_P_H
#define ICORE_P_H

#include <CoreApi/ICoreBase_p.h>

#include "ICore.h"
#include "Preferences/PreferenceRegistry.h"

namespace Core {

    class ICorePrivate : ICoreBasePrivate {
        Q_DECLARE_PUBLIC(ICore)
    public:
        ICorePrivate();

        void init();

        PreferenceRegistry *preferenceRegistry;
    };

}

#endif // ICORE_P_H
