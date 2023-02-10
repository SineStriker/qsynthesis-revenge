#include "QsCoreDecorator.h"
#include "QsLinq.h"
#include "private/QsCoreDecorator_p.h"

#include <QDebug>

Q_SINGLETON_DECLARE(QsCoreDecorator)

QsCoreDecorator::QsCoreDecorator(QObject *parent)
    : QsCoreDecorator(*new QsCoreDecoratorPrivate(), parent) {
}

QsCoreDecorator::~QsCoreDecorator() {
}

// Theme related
QString QsCoreDecorator::theme() const {
    Q_D(const QsCoreDecorator);
    return d->theme;
}

QStringList QsCoreDecorator::themes() const {
    Q_D(const QsCoreDecorator);
    return d->themeNames.keys();
}

void QsCoreDecorator::setTheme(const QString &theme) {
    Q_UNUSED(theme)
}


// Locale related
QString QsCoreDecorator::locale() const {
    Q_D(const QsCoreDecorator);
    return d->loc;
}

QStringList QsCoreDecorator::locales() const {
    Q_D(const QsCoreDecorator);
    return d->localeNames.keys();
}

void QsCoreDecorator::setLocale(const QString &locale) {
    Q_D(QsCoreDecorator);
    if (d->loc == locale) {
        return;
    }
    d->loc = locale;

    // Update translations
    for (const auto &p : qAsConst(d->localeConfigs)) {
        auto &lp = *p;
        if (lp.data.isNull()) {
            continue;
        }
        lp.data->install(d->loc);
    }

    // Notify all
    for (const auto &subscriber : qAsConst(d->localeSubscribers)) {
        subscriber->updater();
    }
}

void QsCoreDecorator::addLocale(const QString &key, const QMap<QString, QStringList> &paths) {
    Q_D(QsCoreDecorator);

    /*

    Steps:
        1. Find value by key in `localeConfigs`
            - If the key exists, it already has subscribers
                - If the `data` has value, it indicates that a repetition occurs, skip
                - If not, implement the empty placeholder
            - If not, insert a placeholder and implement it
        2. If current locale is in the new configuration, the translation
           should be update immediately

    */

    auto setup = [&](LocalePlaceholder &lp) {
        lp.data = QSharedPointer<LocaleData>::create();
        lp.data->qmFiles = QsLinq::MapToHash(paths);

        // Inc ref count
        for (auto it = paths.begin(); it != paths.end(); ++it) {
            d->localeNames[it.key()]++;
        }

        auto it = paths.find(d->loc);
        if (it != paths.end()) {
            // Update
            lp.data->install(it.value());

            // Notify related objects
            for (const auto &sub : qAsConst(lp.subscribers)) {
                sub->updater();
            }
        }
    };

    auto it = d->localeConfigs.find(key);
    if (it != d->localeConfigs.end()) {
        // If there's a placeholder
        auto &lp = *it.value();

        // Repeated
        if (!lp.data.isNull()) {
            return;
        }

        setup(lp);
    } else {
        // Not found
        auto lp = new LocalePlaceholder();
        setup(*lp);

        d->localeConfigs.insert(key, lp);
    }
}

void QsCoreDecorator::removeLocale(const QString &key) {
    Q_D(QsCoreDecorator);

    /*

    Steps:
        1. Ensure the key exists in `localeConfigs`
        2. If the configuration to remove has living translators, the translators should
           be removed immediately

    */

    auto it = d->localeConfigs.find(key);
    if (it == d->localeConfigs.end()) {
        return;
    }

    // Found
    auto &lp = *it.value();

    if (!lp.data.isNull()) {
        const auto &paths = lp.data->qmFiles;

        // Dec ref count
        for (auto it = paths.begin(); it != paths.end(); ++it) {
            auto it2 = d->localeNames.find(it.key());
            it2.value()--;
            if (it2.value() == 0) {
                d->localeNames.erase(it2);
            }
        }

        if (!lp.data->translators.isEmpty()) {
            // Uninstall
            lp.data->uninstall();

            // Notify related objects
            for (const auto &sub : qAsConst(lp.subscribers)) {
                sub->updater();
            }
        }

        lp.data.clear();
    }

    // Remove placeholder if there're no subscribers
    if (lp.subscribers.isEmpty()) {
        delete it.value();
        d->localeConfigs.erase(it);
    }
}

void QsCoreDecorator::installLocale(QObject *obj, const QStringList &keys,
                                    const std::function<void()> updater) {
    Q_D(QsCoreDecorator);

    /*

    Steps:
        1. Ensure it's the first time the widget installs a locale
        2. Go through all keys, if the required configuration doesn't exist, add a empty one as a
    placeholder
        3. Add subscriber to the locale configuration
        4. Add the widget to a global set to check existence
        5. Notify once

    */

    if (d->localeSubscribers.contains(obj)) {
        return;
    }

    auto ls = new LocaleSubscriber();
    ls->keys = keys;
    ls->updater = updater;

    // Add to locale cache
    for (const auto &key : keys) {
        auto setup = [&](LocalePlaceholder &lp) {
            lp.subscribers.insert(ls); //
        };

        auto it2 = d->localeConfigs.find(key);
        if (it2 == d->localeConfigs.end()) {
            // Add new placeholder
            auto lp = new LocalePlaceholder();
            setup(*lp);
            d->localeConfigs.insert(key, lp);
            continue;
        }
        setup(*it2.value());
    }

    // Detect destruction
    connect(obj, &QObject::destroyed, this, &QsCoreDecorator::_q_localeSubscriberDestroyed);

    d->localeSubscribers.insert(obj, ls); // Add subscriber

    // Notify one forcefully
    updater();
}

void QsCoreDecorator::uninstallLocale(QObject *obj) {
    Q_D(QsCoreDecorator);

    /*

    Steps:
        1. Ensure the widget has install a locale configuration
        2. Go through all keys, remove the widget reference from all locale configurations
        3. Remove the widget from the global set

    */

    auto it = d->localeSubscribers.find(obj);
    if (it == d->localeSubscribers.end()) {
        return;
    }

    auto ls = it.value();

    // Remove from locale cache
    for (const auto &key : qAsConst(ls->keys)) {
        auto it2 = d->localeConfigs.find(key);
        if (it2 == d->localeConfigs.end()) {
            continue;
        }

        auto &lp = *it2.value();
        lp.subscribers.remove(ls);

        // Remove placeholder if there're no subscribers
        if (lp.data.isNull() && lp.subscribers.isEmpty()) {
            delete it2.value();
            d->localeConfigs.erase(it2);
        }
    }

    disconnect(obj, &QObject::destroyed, this, &QsCoreDecorator::_q_localeSubscriberDestroyed);

    delete ls;
    d->localeSubscribers.erase(it); // Remove subscriber
}

QsCoreDecorator::QsCoreDecorator(QsCoreDecoratorPrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}

void QsCoreDecorator::_q_localeSubscriberDestroyed() {
    uninstallLocale(sender());
}
