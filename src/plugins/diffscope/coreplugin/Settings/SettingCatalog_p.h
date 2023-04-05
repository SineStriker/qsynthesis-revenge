#ifndef SETTINGCATALOGPRIVATE_H
#define SETTINGCATALOGPRIVATE_H

#include "SettingCatalog.h"

namespace Core {

    class SettingCatalogPrivate {
        Q_DECLARE_PUBLIC(SettingCatalog)
    public:
        SettingCatalogPrivate();
        virtual ~SettingCatalogPrivate();

        void init();

        SettingCatalog *q_ptr;
    };

}

#endif // SETTINGCATALOGPRIVATE_H