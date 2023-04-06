#ifndef SETTINGCATALOGPRIVATE_H
#define SETTINGCATALOGPRIVATE_H

#include "SettingCatalog.h"

#include <Collections/QMChronMap.h>

namespace Core {

    class SettingCatalogPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(SettingCatalog)
    public:
        SettingCatalogPrivate();
        ~SettingCatalogPrivate();

        void init();

        SettingCatalog *q_ptr;

        void addPageRecursive(ISettingPage *page);
        void removePageRecursive(ISettingPage *page);

        QMChronMap<QString, ISettingPage *> pages;
        QHash<QString, QSet<ISettingPage *>> allPages;

    private:
        void _q_pageAdded(ISettingPage *page);
        void _q_pageRemoved(ISettingPage *page);
    };

}

#endif // SETTINGCATALOGPRIVATE_H