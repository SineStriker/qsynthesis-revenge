#ifndef SETTINGCATALOGPRIVATE_H
#define SETTINGCATALOGPRIVATE_H

#include "SettingCatalog.h"

#include <Collections/QMChronMap.h>

namespace Core {

    class SettingCatalogPrivate {
        Q_DECLARE_PUBLIC(SettingCatalog)
    public:
        SettingCatalogPrivate();
        virtual ~SettingCatalogPrivate();

        void init();

        SettingCatalog *q_ptr;

        QMChronMap<QString, ISettingPage *> pages;
    };

}

#endif // SETTINGCATALOGPRIVATE_H