#include "SettingCatalog.h"
#include "SettingCatalog_p.h"

#include <QDebug>

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::SettingCatalog::" << (func) << "():").space()

    SettingCatalogPrivate::SettingCatalogPrivate() {
    }

    SettingCatalogPrivate::~SettingCatalogPrivate() {
    }

    void SettingCatalogPrivate::init() {
    }

    void SettingCatalogPrivate::addPageRecursive(ISettingPage *page) {
        auto doIt = [this](ISettingPage *page) {
            allPages[page->id()].insert(page);
            emit q_ptr->pageAdded(page);

            connect(page, &ISettingPage::titleChanged, this, &SettingCatalogPrivate::_q_pageTitleChanged);
            connect(page, &ISettingPage::descriptionChanged, this, &SettingCatalogPrivate::_q_pageDescriptionChanged);
            connect(page, &ISettingPage::pageAdded, this, &SettingCatalogPrivate::_q_pageAdded);
            connect(page, &ISettingPage::pageRemoved, this, &SettingCatalogPrivate::_q_pageRemoved);
        };

        doIt(page);
        for (const auto &item : page->allPages()) {
            doIt(item);
        }
    }

    void SettingCatalogPrivate::removePageRecursive(ISettingPage *page) {
        auto doIt = [this](ISettingPage *page) {
            auto it = allPages.find(page->id());
            if (it != allPages.end()) {
                auto &set = it.value();
                set.remove(page);
                if (set.isEmpty()) {
                    allPages.erase(it);
                }
            }
            disconnect(page, &ISettingPage::titleChanged, this, &SettingCatalogPrivate::_q_pageTitleChanged);
            disconnect(page, &ISettingPage::descriptionChanged, this,
                       &SettingCatalogPrivate::_q_pageDescriptionChanged);
            disconnect(page, &ISettingPage::pageAdded, this, &SettingCatalogPrivate::_q_pageAdded);
            disconnect(page, &ISettingPage::pageRemoved, this, &SettingCatalogPrivate::_q_pageRemoved);

            emit q_ptr->pageRemoved(page);
        };

        doIt(page);
        for (const auto &item : page->allPages()) {
            doIt(item);
        }
    }

    void SettingCatalogPrivate::_q_pageTitleChanged(const QString &title) {
        Q_Q(SettingCatalog);
        emit q->titleChanged(qobject_cast<ISettingPage *>(sender()), title);
    }

    void SettingCatalogPrivate::_q_pageDescriptionChanged(const QString &desc) {
        Q_Q(SettingCatalog);
        emit q->descriptionChanged(qobject_cast<ISettingPage *>(sender()), desc);
    }

    void SettingCatalogPrivate::_q_pageAdded(ISettingPage *page) {
        addPageRecursive(page);
    }

    void SettingCatalogPrivate::_q_pageRemoved(ISettingPage *page) {
        removePageRecursive(page);
    }

    SettingCatalog::SettingCatalog(QObject *parent) : SettingCatalog(*new SettingCatalogPrivate(), parent) {
    }

    SettingCatalog::~SettingCatalog() {
    }

    bool SettingCatalog::addPage(ISettingPage *page) {
        Q_D(SettingCatalog);
        if (!page) {
            myWarning(__func__) << "trying to add null page";
            return false;
        }
        if (d->pages.contains(page->id())) {
            myWarning(__func__) << "trying to add duplicated page:" << page->id();
            return false;
        }

        page->setParent(this);
        d->pages.append(page->id(), page);

        d->addPageRecursive(page);

        return true;
    }

    bool SettingCatalog::removePage(ISettingPage *page) {
        Q_D(SettingCatalog);
        if (page == nullptr) {
            myWarning(__func__) << "trying to remove null page";
            return false;
        }
        return removePage(page->id());
    }

    bool SettingCatalog::removePage(const QString &id) {
        Q_D(SettingCatalog);
        auto it = d->pages.find(id);
        if (it == d->pages.end()) {
            myWarning(__func__) << "page does not exist:" << id;
            return false;
        }

        auto page = it.value();
        page->setParent(nullptr);
        d->pages.erase(it);

        d->removePageRecursive(page);

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

    QList<ISettingPage *> SettingCatalog::pages(const QString &id) const {
        Q_D(const SettingCatalog);
        return d->allPages.value(id, {}).values();
    }

    QList<ISettingPage *> SettingCatalog::allPages() const {
        Q_D(const SettingCatalog);
        QList<ISettingPage *> res;
        for (const auto &page : d->pages) {
            res.append(page);
            res.append(page->allPages());
        }
        return res;
    }

    SettingCatalog::SettingCatalog(SettingCatalogPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
