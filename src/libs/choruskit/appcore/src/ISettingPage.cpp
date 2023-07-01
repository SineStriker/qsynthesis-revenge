#include "ISettingPage.h"
#include "ISettingPage_p.h"

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::ISettingPage::" << (func) << "():").space()

    ISettingPagePrivate::ISettingPagePrivate() {
    }

    ISettingPagePrivate::~ISettingPagePrivate() {
    }

    void ISettingPagePrivate::init() {
    }

    ISettingPage::ISettingPage(const QString &id, QObject *parent)
        : ISettingPage(*new ISettingPagePrivate(), id, parent) {
    }

    ISettingPage::~ISettingPage() {
    }

    QString ISettingPage::id() const {
        Q_D(const ISettingPage);
        return d->id;
    }

    QString ISettingPage::title() const {
        Q_D(const ISettingPage);
        return d->title;
    }

    void ISettingPage::setTitle(const QMDisplayString &title) {
        Q_D(ISettingPage);
        d->title = title;

        emit titleChanged(title);
    }

    QString ISettingPage::description() const {
        Q_D(const ISettingPage);
        return d->description;
    }

    void ISettingPage::setDescription(const QMDisplayString &description) {
        Q_D(ISettingPage);
        d->description = description;

        emit descriptionChanged(description);
    }

    bool ISettingPage::addPage(ISettingPage *page) {
        Q_D(ISettingPage);
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
        emit pageAdded(page);

        return true;
    }

    bool ISettingPage::removePage(ISettingPage *page) {
        Q_D(ISettingPage);
        if (page == nullptr) {
            myWarning(__func__) << "trying to remove null page";
            return false;
        }
        return removePage(page->id());
    }

    bool ISettingPage::removePage(const QString &id) {
        Q_D(ISettingPage);
        auto it = d->pages.find(id);
        if (it == d->pages.end()) {
            myWarning(__func__) << "page does not exist:" << id;
            return false;
        }

        auto page = it.value();
        page->setParent(nullptr);
        d->pages.erase(it);
        emit pageRemoved(page);

        return true;
    }

    ISettingPage *ISettingPage::page(const QString &id) const {
        Q_D(const ISettingPage);
        return d->pages.value(id, nullptr);
    }

    QList<ISettingPage *> ISettingPage::pages() const {
        Q_D(const ISettingPage);
        return d->pages.values();
    }

    QList<ISettingPage *> ISettingPage::allPages() const {
        Q_D(const ISettingPage);
        QList<ISettingPage *> res;
        for (const auto &page : d->pages) {
            res.append(page);
            res.append(page->allPages());
        }
        return res;
    }

    QString ISettingPage::sortKeyword() const {
        Q_D(const ISettingPage);
        return d->id;
    }

    bool ISettingPage::matches(const QString &word) const {
        Q_D(const ISettingPage);
        return d->title.text().contains(word, Qt::CaseInsensitive) || sortKeyword().contains(word, Qt::CaseInsensitive);
    }

    ISettingPage::ISettingPage(ISettingPagePrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;
        d.title = id;

        d.init();
    }

}