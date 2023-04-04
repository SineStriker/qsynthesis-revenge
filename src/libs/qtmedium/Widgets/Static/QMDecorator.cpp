#include "QMDecorator.h"
#include "private/QMDecorator_p.h"
#include "private/ThemeGuard.h"

#include <QApplication>
#include <QDebug>
#include <QWidget>

#include <algorithm>

QMDecorator::QMDecorator(QObject *parent) : QMDecorator(*new QMDecoratorPrivate(), parent) {
}

QMDecorator::~QMDecorator() {
}

void QMDecorator::setTheme(const QString &theme) {
    Q_D(QMDecorator);

    if (d->theme == theme) {
        return;
    }
    d->theme = theme;

    // Invalid all
    QSet<ThemeSubscriber *> subscribersToUpdate;
    for (auto p : qAsConst(d->themeTemplates)) {
        auto &tp = *p;
        tp.invalidate();

        for (const auto &sub : qAsConst(tp.subscribers)) {
            subscribersToUpdate.insert(sub);
        }
    }

    // Notify all
    for (const auto &sub : qAsConst(subscribersToUpdate)) {
        sub->notifyAll();
    }
}

void QMDecorator::addThemeTemplate(const QString &key, const QString &path) {
    Q_D(QMDecorator);

    /*

    Steps:
        1. Find value by key in `themeTemplates`
            - If the key exists, it already has subscribers or configs
                - If the `data` has value, it indicates that a repetition occurs, skip
                - If not, implement the empty placeholder
            - If not, insert a placeholder and implement it
        2. If the dirty theme keys contains current theme, notify all related widgets

    */

    auto setup = [&](ThemePlaceholder &tp, ThemeTemplate &tt) {
        if (tp.data.isNull()) {
            tp.data = QSharedPointer<ThemeTemplate>::create(std::move(tt));
        }

        const auto &keys = tp.configs.keys();
        if (keys.contains(d->theme)) {
            tp.invalidate();

            // Notify related objects
            for (const auto &sub : qAsConst(tp.subscribers)) {
                sub->notifyAll();
            }
        }
    };

    ThemeTemplate tt;
    if (!tt.load(path)) {
        return;
    }

    auto it = d->themeTemplates.find(key);
    if (it != d->themeTemplates.end()) {
        // If there's a placeholder
        auto &tp = *it.value();

        // Repeated
        if (!tp.data.isNull()) {
            return;
        }

        setup(tp, tt);
    } else {
        // Not found
        auto tp = new ThemePlaceholder();
        tp->ns = key;
        setup(*tp, tt);

        d->themeTemplates.insert(key, tp);
    }
}

void QMDecorator::removeThemeTemplate(const QString &key) {
    Q_D(QMDecorator);

    /*

    Steps:
        1. Ensure the key exists in `themeTemplates`
        2. If the dirty theme keys contains current theme, notify all related widgets

    */

    auto it = d->themeTemplates.find(key);
    if (it == d->themeTemplates.end()) {
        return;
    }

    // Found
    auto &tp = *it.value();

    if (!tp.data.isNull()) {
        tp.data.clear();

        if (tp.configs.contains(d->theme)) {
            // Notify related objects
            for (const auto &sub : qAsConst(tp.subscribers)) {
                sub->notifyAll();
            }
        }
    }

    // Remove placeholder if there're no subscribers
    if (tp.subscribers.isEmpty() && tp.configs.isEmpty()) {
        delete it.value();
        d->themeTemplates.erase(it);
    }
}

void QMDecorator::addThemeConfig(const QString &key, const QMap<QString, QStringList> &paths) {
    Q_D(QMDecorator);

    /*

    Steps:
        1. Ensure no duplication
        2. Go through all namespaces, if the required template doesn't exist, add a empty one as a
           placeholder
        3. Add config to the template
        4. Add the config to a global set
        5. Notify related widgets after the templates update

    */

    if (d->themeConfigs.contains(key)) {
        return;
    }

    QSet<ThemePlaceholder *> templatesToUpdate;

    ThemeConfigPack pack;
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        const QString &themeKey = it.key();
        const QStringList &themeConfs = it.value();

        // Load config
        auto conf = QSharedPointer<ThemeConfig>::create();
        if (!conf->load(themeConfs)) {
            continue;
        }

        // Inc ref count
        d->themeNames[themeKey]++;

        // Add to configs
        pack.data.insert(themeKey, conf);

        auto setup = [&](ThemePlaceholder &tp) {
            auto &map = tp.configs[themeKey].data[conf->priority];
            auto it = map.insert(key, conf);

            // Dirty if first
            if (
#if !ENABLE_PARTIAL_OVERRIDE
                it == map.begin() &&
#endif
                themeKey == d->theme) {
                templatesToUpdate.insert(&tp);
            }
        };

        // Add reference to templates
        for (const auto &ns : qAsConst(conf->namespaces)) {
            auto it2 = d->themeTemplates.find(ns);
            if (it2 == d->themeTemplates.end()) {
                // Add new placeholder
                auto tp = new ThemePlaceholder();
                tp->ns = ns;
                setup(*tp);
                d->themeTemplates.insert(ns, tp);
                continue;
            }
            setup(*it2.value());
        }
    }

    d->themeConfigs.insert(key, pack);

    QSet<ThemeSubscriber *> subscribersToUpdate;
    for (auto p : qAsConst(templatesToUpdate)) {
        auto &tp = *p;
        tp.invalidate();

        // Notify related objects
        for (const auto &sub : qAsConst(tp.subscribers)) {
            subscribersToUpdate.insert(sub);
        }
    }

    for (const auto &sub : qAsConst(subscribersToUpdate)) {
        sub->notifyAll();
    }
}

void QMDecorator::removeThemeConfig(const QString &key) {
    Q_D(QMDecorator);

    /*

    Steps:
        1. Ensure the key exists
        2. Go through all keys, remove the config reference from all templates
        3. Remove the widget from the global set
        4. Notify related widgets after the templates update

    */

    auto it = d->themeConfigs.find(key);
    if (it == d->themeConfigs.end()) {
        return;
    }

    QSet<ThemePlaceholder *> templatesToUpdate;

    const auto &pack = it.value();
    for (auto it1 = pack.data.begin(); it1 != pack.data.end(); ++it1) {
        const auto &themeKey = it1.key();
        const auto &conf = it1.value();

        // Dec ref count
        {
            auto it2 = d->themeNames.find(themeKey);
            it2.value()--;
            if (it2.value() == 0) {
                d->themeNames.erase(it2);
            }
        }

        // Remove reference from templates
        for (const auto &ns : conf->namespaces) {
            auto it2 = d->themeTemplates.find(ns);
            if (it2 == d->themeTemplates.end()) {
                continue;
            }
            auto &tp = *d->themeTemplates[ns];
            auto &map0 = tp.configs[themeKey].data;

            auto it3 = map0.find(conf->priority);
            Q_ASSERT(it3 != map0.end());

            auto &map = it3->second;
            auto it4 = map.find(key);

#if !ENABLE_PARTIAL_OVERRIDE
            // Save status
            bool beg = it4 == map.begin();
#endif

            map.erase(it4);
            if (map.isEmpty()) {
                map0.erase(it3);
            }

            // Remove placeholder if there're no subscribers
            if (tp.isEmpty()) {
                delete it2.value();
                d->themeTemplates.erase(it2);
            } else if (
#if !ENABLE_PARTIAL_OVERRIDE
                beg &&
#endif
                themeKey == d->theme) {
                // Dirty if first
                templatesToUpdate.insert(&tp);
            }
        }
    }

    d->themeConfigs.erase(it);

    QSet<ThemeSubscriber *> subscribersToUpdate;
    for (auto p : qAsConst(templatesToUpdate)) {
        auto &tp = *p;
        tp.invalidate();

        // Notify related objects
        for (const auto &sub : qAsConst(tp.subscribers)) {
            subscribersToUpdate.insert(sub);
        }
    }

    for (const auto &sub : qAsConst(subscribersToUpdate)) {
        sub->notifyAll();
    }
}

void QMDecorator::installTheme(QWidget *w, const QStringList &templateKeys) {
    Q_D(QMDecorator);

    if (d->themeSubscribers.contains(w)) {
        return;
    }

    ThemeSubscriber *ts;
    auto &keySeq = templateKeys;

    // Find group by key set
    auto it = d->themeSubscriberGroups.find(keySeq);
    if (it == d->themeSubscriberGroups.end()) {
        // Add new
        ts = new ThemeSubscriber();
        ts->keySeq = keySeq;
        d->themeSubscriberGroups.insert(keySeq, ts);

        // Add to theme cache
        for (const auto &key : qAsConst(keySeq)) {
            auto setup = [&](ThemePlaceholder &tp) {
                tp.subscribers.insert(ts);
                ts->templates.insert(key, &tp);
            };

            auto it2 = d->themeTemplates.find(key);
            if (it2 == d->themeTemplates.end()) {
                // Add new placeholder
                auto tp = new ThemePlaceholder();
                tp->ns = key;
                setup(*tp);
                d->themeTemplates.insert(key, tp);
                continue;
            }
            setup(*it2.value());
        }
    } else {
        // Fetch
        ts = it.value();
    }

    // Add to global set
    d->themeSubscribers.insert(w, ts);

    // Add guard to group and notify
    ts->addWidget(w);

    // Detect destruction
    connect(w, &QObject::destroyed, this, &QMDecorator::_q_themeSubscriberDestroyed);
}

void QMDecorator::uninstallTheme(QWidget *w) {
    Q_D(QMDecorator);

    auto it = d->themeSubscribers.find(w);
    if (it == d->themeSubscribers.end()) {
        return;
    }

    auto ts = it.value();

    // Remove guard from group
    ts->removeWidget(w);

    // Remove from global set
    d->themeSubscribers.erase(it);

    // Remove group if no widgets
    if (ts->isEmpty()) {
        auto keys = ts->templates.keys();
        auto &keySeq = keys;

        // Remove from theme cache
        for (const auto &key : qAsConst(keySeq)) {
            auto it2 = d->themeTemplates.find(key);
            if (it2 == d->themeTemplates.end()) {
                continue;
            }

            auto &tp = *it2.value();
            tp.subscribers.remove(ts);

            // Remove placeholder if there're no subscribers
            if (tp.isEmpty()) {
                d->themeTemplates.erase(it2);
            }
        }

        delete ts;                               // Remove subscriber
        d->themeSubscriberGroups.remove(keySeq); // Remove group
    }

    disconnect(w, &QObject::destroyed, this, &QMDecorator::_q_themeSubscriberDestroyed);
}

// Protected or private members
QMDecorator::QMDecorator(QMDecoratorPrivate &d, QObject *parent) : QMCoreDecorator(d, parent) {
    d.init();
}

void QMDecorator::_q_screenAdded(QScreen *screen) {
    connect(screen, &QScreen::physicalDotsPerInchChanged, this, &QMDecorator::_q_deviceRatioChanged);
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &QMDecorator::_q_logicalRatioChanged);
}

void QMDecorator::_q_screenRemoved(QScreen *screen) {
    disconnect(screen, &QScreen::physicalDotsPerInchChanged, this,
               &QMDecorator::_q_deviceRatioChanged);
    disconnect(screen, &QScreen::logicalDotsPerInchChanged, this,
               &QMDecorator::_q_logicalRatioChanged);

    // How to deal with windows on the removed screen?
}

void QMDecorator::_q_deviceRatioChanged(double dpi) {
    Q_D(QMDecorator);
    auto screen = qobject_cast<QScreen *>(sender());
    d->screenChange_helper(screen);
    emit deviceRatioChanged(screen, dpi);
}

void QMDecorator::_q_logicalRatioChanged(double dpi) {
    Q_D(QMDecorator);
    auto screen = qobject_cast<QScreen *>(sender());
    d->screenChange_helper(screen);
    emit logicalRatioChanged(screen, dpi);
}

void QMDecorator::_q_themeSubscriberDestroyed() {
    auto w = qobject_cast<QWidget *>(sender());
    uninstallTheme(w);
}
