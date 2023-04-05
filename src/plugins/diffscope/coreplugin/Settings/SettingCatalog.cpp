#include "SettingCatalog.h"
#include "SettingCatalog_p.h"

namespace Core {

    SettingCatalogPrivate::SettingCatalogPrivate() {
    }

    SettingCatalogPrivate::~SettingCatalogPrivate() {
    }

    void SettingCatalogPrivate::init() {
    }

    SettingCatalog::SettingCatalog(QObject *parent) : SettingCatalog(*new SettingCatalogPrivate(), parent) {
    }

    SettingCatalog::~SettingCatalog() {
    }

    bool SettingCatalog::addCatalog(const QStringList &treePath, ISettingPage *page) {
        return false;
    }

    bool SettingCatalog::removeCatalog(const QStringList &treePath) {
        return false;
    }

    bool SettingCatalog::loadCatalogs(const QString &filename, const QList<ISettingPage *> &pages) {
        return false;
    }

    SettingCatalog::SettingCatalog(SettingCatalogPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
