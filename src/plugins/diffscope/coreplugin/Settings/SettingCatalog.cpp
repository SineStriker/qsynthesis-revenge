#include "SettingCatalog.h"
#include "SettingCatalog_p.h"

#include <QDebug>

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

    bool SettingCatalog::addPage(ISettingPage *page) {
        Q_D(SettingCatalog);
        if (!page) {
            qWarning() << "Core::ISettingPage::addPage(): trying to add null page";
            return false;
        }
        if (d->pages.contains(page->id())) {
            qWarning() << "Core::ISettingPage::addPage(): trying to add duplicated page:" << page->id();
            return false;
        }

        page->setParent(this);
        d->pages.append(page->id(), page);
        return true;
    }

    bool SettingCatalog::removePage(ISettingPage *page) {
        Q_D(SettingCatalog);
        if (page == nullptr) {
            qWarning() << "Core::ISettingPage::removePage(): trying to remove null page";
            return false;
        }
        return removePage(page->id());
    }

    bool SettingCatalog::removePage(const QString &id) {
        Q_D(SettingCatalog);
        auto it = d->pages.find(id);
        if (it == d->pages.end()) {
            qWarning() << "Core::ISettingPage::removePage(): page does not exist:" << id;
            return false;
        }

        auto context = it.value();
        context->setParent(nullptr);
        d->pages.erase(it);

        return true;
    }

    ISettingPage *SettingCatalog::page(const QString &id) const {
        Q_D(const SettingCatalog);
        return d->pages.value(id, nullptr);
    }

    QList<ISettingPage *> SettingCatalog::pages() const {
        Q_D(const SettingCatalog);
        return d->pages.values();
    }

    SettingCatalog::SettingCatalog(SettingCatalogPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
