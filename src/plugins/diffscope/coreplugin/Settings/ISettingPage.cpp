#include "ISettingPage.h"
#include "ISettingPage_p.h"

namespace Core {

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

    void ISettingPage::setTitle(const QString &title) {
        Q_D(ISettingPage);
        d->title = title;

        emit titleChanged(title);
    }

    bool ISettingPage::matches(const QString &word) const {
        return false;
    }

    ISettingPage::ISettingPage(ISettingPagePrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();

        d.id = id;
        d.title = id;
    }

}