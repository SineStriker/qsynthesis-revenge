#ifndef ICOREBASEPRIVATE_H
#define ICOREBASEPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the ChorusKit API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

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
        DocumentSystem *documentSystem;
        SettingCatalog *settingCatalog;

        ICoreBase *q_ptr;
    };

}

#endif // ICOREBASEPRIVATE_H