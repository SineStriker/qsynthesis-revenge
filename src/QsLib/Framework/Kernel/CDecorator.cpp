#include "CDecorator.h"
#include "private/CDecorator_p.h"

#include <QApplication>

Q_SINGLETON_DECLARE(CDecorator)

CDecorator::CDecorator(QObject *parent) : CDecorator(*new CDecoratorPrivate(), parent) {
}

CDecorator::~CDecorator() {
}

// Locale related
int CDecorator::locale() const {
    Q_D(const CDecorator);
    return d->loc;
}

void CDecorator::setLocale(int locale) {
    Q_D(CDecorator);

    int org = d->loc;
    if (org == locale) {
        return;
    }
    d->loc = locale;

    // Notify all
    for (const auto &subscriber : qAsConst(d->localeSubscribers)) {
        subscriber.updater();
    }
}

void CDecorator::addLocale(const QString &key, const QMap<int, QStringList> &qm) {
    Q_D(CDecorator);

    auto setup = [&](LocaleData &ld) {
        ld.qmFiles = qm; //
    };

    auto it = d->locales.find(key);
    if (it != d->locales.end()) {
        // Found
        auto &ld = it.value();
        setup(ld);

        // Notify related objects
        for (const auto &subscriber : qAsConst(ld.subscribers)) {
            auto it2 = d->localeSubscribers.find(subscriber);
            if (it2 == d->localeSubscribers.end()) {
                continue;
            }
            it2.value().updater();
        }
    } else {
        // Not found
        LocaleData ld;
        setup(ld);

        // Insert
        d->locales.insert(key, ld);
    }
}

void CDecorator::removeLocale(const QString &key) {
    Q_D(CDecorator);

    auto it = d->locales.find(key);
    if (it == d->locales.end()) {
        return;
    }

    // Found
    auto &ld = it.value();

    // Notify related objects
    for (const auto &subscriber : qAsConst(ld.subscribers)) {
        auto it2 = d->localeSubscribers.find(subscriber);
        if (it2 == d->localeSubscribers.end()) {
            continue;
        }
        it2.value().updater();
    }

    // Remove
    d->locales.erase(it);
}

void CDecorator::installLocale(QWidget *w, const QStringList &keys,
                               const std::function<void()> updater) {
    Q_D(CDecorator);

    LocaleSubscriber ls;
    ls.keys = keys;
    ls.updater = updater;

    // Add to locale cache
    for (const auto &key : keys) {
        auto it2 = d->locales.find(key);
        if (it2 == d->locales.end()) {
            LocaleData ld;
            ld.subscribers.insert(w);
            d->locales.insert(key, ld);
            continue;
        }
        it2.value().subscribers.insert(w);
    }

    // Detect destruction
    connect(w, &QObject::destroyed, this, &CDecorator::_q_localeSubscriberDestroyed);

    // Add
    d->localeSubscribers.insert(w, ls);

    // Notify one
    updater();
}

void CDecorator::uninstallLocale(QWidget *w) {
    Q_D(CDecorator);

    auto it = d->localeSubscribers.find(w);
    if (it == d->localeSubscribers.end()) {
        return;
    }

    // Remove from locale cache
    for (const auto &key : it->keys) {
        auto it2 = d->locales.find(key);
        if (it2 == d->locales.end()) {
            continue;
        }
        it2.value().subscribers.remove(w);
    }

    disconnect(w, &QObject::destroyed, this, &CDecorator::_q_localeSubscriberDestroyed);

    // Remove
    d->localeSubscribers.erase(it);
}


// Theme related
int CDecorator::theme() const {
    Q_D(const CDecorator);
    return d->theme;
}

void CDecorator::setTheme(int theme) {
    Q_D(CDecorator);

    int org = d->theme;
    if (org == theme) {
        return;
    }
    d->theme = theme;

    // Notify all
    for (auto it = d->themeSubscribers.begin(); it != d->themeSubscribers.end()) {
        it->filter->updateScreen();
    }
}

void CDecorator::addThemeTemplate(const QString &key, const QString &path) {
}

void CDecorator::removeThemeTemplate(const QString &key) {
}

void CDecorator::addThemeConfig(const QString &key, const QMap<int, QStringList> &conf) {
    Q_D(CDecorator);

    auto setup = [&](ThemeData &td) {
        td.configs = conf;
        for (const auto &path : templates) {
            ThemeTemplate tt;
            if (!tt.load(path)) {
                continue;
            }
            td.templates.insert(tt.ns, tt);

            // Use latest configuration
            d->themeNamespaces.insert(tt.ns, key);
        }
    };

    auto it = d->themes.find(key);
    if (it != d->themes.end()) {
        auto &td = it.value();
        setup(td);

        // Notify related objects
        for (const auto &subscriber : qAsConst(td.subscribers)) {
            auto it2 = d->themeSubscribers.find(subscriber);
            if (it2 == d->themeSubscribers.end()) {
                continue;
            }
            it2->filter->updateScreen();
        }
    } else {
        /**
         * @brief Unrechable code
         *
         */

        // Not found
        ThemeData ld;
        setup(ld);

        // Insert
        d->themes.insert(key, ld);
    }
}

void CDecorator::removeThemeConfig(const QString &key) {
    Q_D(CDecorator);

    auto it = d->themes.find(key);
    if (it == d->themes.end()) {
        return;
    }

    // Found
    auto &td = it.value();

    // Notify related objects
    for (const auto &subscriber : qAsConst(td.subscribers)) {
        auto it2 = d->themeSubscribers.find(subscriber);
        if (it2 == d->themeSubscribers.end()) {
            continue;
        }
        it2.value().filter->updateScreen();
    }

    // Remove
    d->themes.erase(it);
}

void CDecorator::installTheme(QWidget *w, const QStringList &namespaces) {
    Q_D(CDecorator);

    ThemeSubscriber ts;
    auto df = ts.filter = new ThemeSubscriber(w, this, d);
    df->namespaces = namespaces;

    // Add to theme cache
    for (const auto &ns : namespaces) {
        auto it2 = d->themeNamespaces.find(ns);
        if (it2 == d->themeNamespaces.end()) {
            continue;
        }

        auto it3 = d->themes.find(*it2);
        if (it3 == d->themes.end()) {
            continue;
        }

        (*it3).subscribers.insert(w);
    }

    // Detect destruction
    connect(w, &QObject::destroyed, this, &CDecorator::_q_themeSubscriberDestroyed);

    // Add
    d->themeSubscribers.insert(w, ts);

    // Notify one
    df->updateScreen();
}

void CDecorator::uninstallTheme(QWidget *w) {
    Q_D(CDecorator);

    auto it = d->themeSubscribers.find(w);
    if (it == d->themeSubscribers.end()) {
        return;
    }

    auto df = it->filter;

    // Remove from theme cache
    for (const auto &key : df->namespaces) {
        auto it2 = d->themes.find(key);
        if (it2 == d->themes.end()) {
            continue;
        }
        it2.value().subscribers.remove(w);
    }

    disconnect(w, &QObject::destroyed, this, &CDecorator::_q_themeSubscriberDestroyed);

    delete df;

    // Remove
    d->themeSubscribers.erase(it);
}

int CDecorator::registerNewTheme() {
    Q_D(CDecorator);
    return ++d->themeMax;
}

// Protected or private members
CDecorator::CDecorator(CDecoratorPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

void CDecorator::_q_screenAdded(QScreen *screen) {
    connect(screen, &QScreen::physicalDotsPerInchChanged, this, &CDecorator::_q_deviceRatioChanged);
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &CDecorator::_q_logicalRatioChanged);
}

void CDecorator::_q_screenRemoved(QScreen *screen) {
    disconnect(screen, &QScreen::physicalDotsPerInchChanged, this,
               &CDecorator::_q_deviceRatioChanged);
    disconnect(screen, &QScreen::logicalDotsPerInchChanged, this,
               &CDecorator::_q_logicalRatioChanged);
}

void CDecorator::_q_deviceRatioChanged(double dpi) {
    emit deviceRatioChanged(qobject_cast<QScreen *>(sender()), dpi);
}

void CDecorator::_q_logicalRatioChanged(double dpi) {
    emit logicalRatioChanged(qobject_cast<QScreen *>(sender()), dpi);
}

void CDecorator::_q_localeSubscriberDestroyed() {
    auto w = qobject_cast<QWidget *>(sender());
    uninstallLocale(w);
}


void CDecorator::_q_themeSubscriberDestroyed() {
    auto w = qobject_cast<QWidget *>(sender());
    uninstallTheme(w);
}
