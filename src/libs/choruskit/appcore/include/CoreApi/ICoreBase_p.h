#ifndef ICOREBASEPRIVATE_H
#define ICOREBASEPRIVATE_H

#include "ICoreBase.h"

namespace Core{

    class CKAPPCORE_API ICoreBasePrivate {
        Q_DECLARE_PUBLIC(ICoreBase)
    public:
        ICoreBasePrivate();
        virtual ~ICoreBasePrivate();

        void init();

        ActionSystem *actionSystem;
        WindowSystem *windowSystem;
        DialogHelper *dialogHelper;
        SettingCatalog *settingCatalog;

        ICoreBase *q_ptr;
    };

}

#endif // ICOREBASEPRIVATE_H