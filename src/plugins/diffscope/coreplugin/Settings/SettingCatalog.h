#ifndef SETTINGCATALOG_H
#define SETTINGCATALOG_H

#include "ISettingPage.h"

namespace Core {

    class SettingCatalogPrivate;

    class CORE_EXPORT SettingCatalog : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SettingCatalog)
    public:
        explicit SettingCatalog(QObject *parent = nullptr);
        ~SettingCatalog();

    public:
        bool addPage(ISettingPage *page);
        bool removePage(ISettingPage *page);
        bool removePage(const QString &id);

        QList<ISettingPage *> pages(const QString &id) const;
        QList<ISettingPage *> allPages() const;

    signals:
        void pageAdded(ISettingPage *page);
        void pageRemoved(ISettingPage *page);

    protected:
        SettingCatalog(SettingCatalogPrivate &d, QObject *parent = nullptr);

        QScopedPointer<SettingCatalogPrivate> d_ptr;
    };

}

#endif // SETTINGCATALOG_H