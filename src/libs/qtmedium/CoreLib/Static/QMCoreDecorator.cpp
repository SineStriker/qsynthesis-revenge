#include "QMCoreDecorator.h"
#include "QMLinq.h"
#include "private/QMCoreDecorator_p.h"

#include <QDebug>

Q_SINGLETON_DECLARE(QMCoreDecorator)

QMCoreDecorator::QMCoreDecorator(QObject *parent) : QMCoreDecorator(*new QMCoreDecoratorPrivate(), parent) {
}

QMCoreDecorator::~QMCoreDecorator() {
}

// Theme related
QString QMCoreDecorator::theme() const {
    Q_D(const QMCoreDecorator);
    return d->theme;
}

QStringList QMCoreDecorator::themes() const {
    Q_D(const QMCoreDecorator);
    return d->themeNames.keys();
}

void QMCoreDecorator::setTheme(const QString &theme) {
    emit themeChanged(theme);
}


// Locale related
QString QMCoreDecorator::locale() const {
    Q_D(const QMCoreDecorator);
    return d->loc;
}

QStringList QMCoreDecorator::locales() const {
    Q_D(const QMCoreDecorator);
    return d->localeNames.keys();
}

void QMCoreDecorator::setLocale(const QString &locale) {
    Q_D(QMCoreDecorator);
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
        for (const auto &updater : subscriber->allUpdaters) {
            updater();
        }
    }

    emit localeChanged(locale);
}

void QMCoreDecorator::addLocale(const QString &key, const QMap<QString, QStringList> &paths) {
    Q_D(QMCoreDecorator);

    /*

    Steps:
        1. Find value by key in `localeConfigs`
            - If the key exists, it already has subscribers
                - If the `data` has value, it indicates that a repetition occurs, skip
                - If not, implement the empty placeholder
            - If not, insert a placeholder and implement it
        2. If current locale is in the new configuration, the translation
           should be updated immediately

    */

    auto setup = [&](LocalePlaceholder &lp) {
        lp.data = QSharedPointer<LocaleData>::create();
        lp.data->qmFiles = QMLinq::MapToHash(paths);

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
                auto updaters = sub->updaters.value(key, {});
                for (const auto &updater : qAsConst(updaters)) {
                    updater();
                }
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

    // Update all global decorators
    {
        auto it2 = d->localeConfigs.find({});
        if (it2 != d->localeConfigs.end()) {
            auto &lp = *it2.value();
            // Notify related objects
            for (const auto &sub : qAsConst(lp.subscribers)) {
                auto updaters = sub->updaters.value(key, {});
                for (const auto &updater : qAsConst(updaters)) {
                    updater();
                }
            }
        }
    }
}

void QMCoreDecorator::removeLocale(const QString &key) {
    Q_D(QMCoreDecorator);

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
                auto updaters = sub->updaters.value(key, {});
                for (const auto &updater : qAsConst(updaters)) {
                    updater();
                }
            }
        }

        lp.data.clear();
    }

    // Remove placeholder if there are no subscribers
    if (lp.subscribers.isEmpty()) {
        delete it.value();
        d->localeConfigs.erase(it);
    }

    // Update all global decorators
    {
        auto it2 = d->localeConfigs.find({});
        if (it2 != d->localeConfigs.end()) {
            auto &lp = *it2.value();
            // Notify related objects
            for (const auto &sub : qAsConst(lp.subscribers)) {
                auto updaters = sub->updaters.value(key, {});
                for (const auto &updater : qAsConst(updaters)) {
                    updater();
                }
            }
        }
    }
}

void QMCoreDecorator::installLocale(QObject *obj, const QStringList &keys, const std::function<void()> &updater) {
    Q_D(QMCoreDecorator);

    /*

    Steps:
        1. Check if the subscriber exists, create if not
        2. Go through all keys, if the required configuration doesn't exist, add an empty one as a
    placeholder
        3. Add subscriber to the locale configuration
        4. Add the widget to a global set to check existence
        5. Notify once

    */

    auto ls = d->localeSubscribers.value(obj, nullptr);
    if (!ls) {
        ls = new LocaleSubscriber();

        // Detect destruction
        connect(obj, &QObject::destroyed, d, &QMCoreDecoratorPrivate::_q_localeSubscriberDestroyed);

        d->localeSubscribers.insert(obj, ls); // Add subscriber
    }

    for (const auto &key : keys) {
        ls->updaters[key].push_back(updater);
    }
    ls->allUpdaters.push_back(updater);

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

    // Notify one forcefully
    updater();
}

void QMCoreDecorator::uninstallLocale(QObject *obj) {
    Q_D(QMCoreDecorator);

    /*

    Steps:
        1. Ensure the widget has installed a locale configuration
        2. Go through all keys, remove the widget reference from all locale configurations
        3. Remove the widget from the global set

    */

    auto it = d->localeSubscribers.find(obj);
    if (it == d->localeSubscribers.end()) {
        return;
    }

    auto ls = it.value();

    // Remove from locale cache
    const auto &keys = ls->updaters.keys();
    for (const auto &key : keys) {
        auto it2 = d->localeConfigs.find(key);
        if (it2 == d->localeConfigs.end()) {
            continue;
        }

        auto &lp = *it2.value();
        lp.subscribers.remove(ls);

        // Remove placeholder if there are no subscribers
        if (lp.data.isNull() && lp.subscribers.isEmpty()) {
            delete it2.value();
            d->localeConfigs.erase(it2);
        }
    }

    disconnect(obj, &QObject::destroyed, d, &QMCoreDecoratorPrivate::_q_localeSubscriberDestroyed);

    delete ls;
    d->localeSubscribers.erase(it); // Remove subscriber
}

QMCoreDecorator::QMCoreDecorator(QMCoreDecoratorPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}
