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
        bool addCatalog(const QStringList &treePath, ISettingPage *page);
        bool removeCatalog(const QStringList &treePath);

        bool loadCatalogs(const QString &filename, const QList<ISettingPage *> &pages);

    protected:
        SettingCatalog(SettingCatalogPrivate &d, QObject *parent = nullptr);

        QScopedPointer<SettingCatalogPrivate> d_ptr;
    };

}

#endif // SETTINGCATALOG_H