#include "CDecorator.h"
#include "private/CDecorator_p.h"
#include "private/ThemeGuard.h"

#include <QApplication>
#include <QDebug>
#include <QWidget>

CDecorator::CDecorator(QObject *parent) : CDecorator(*new CDecoratorPrivate(), parent) {
}

CDecorator::~CDecorator() {
}

// Theme related
QString CDecorator::theme() const {
    Q_D(const CDecorator);
    return d->theme;
}

QStringList CDecorator::themes() const {
    Q_D(const CDecorator);
    return d->themeNames.keys();
}

void CDecorator::setTheme(const QString &theme) {
    Q_D(CDecorator);

    if (d->theme == theme) {
        return;
    }
    d->theme = theme;

    // Notify all
}

void CDecorator::addThemeTemplate(const QString &key, const QString &path) {
    Q_D(CDecorator);

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
        tp.dirtyThemeKeys = QSet<QString>(keys.begin(), keys.end());
        tp.invalidate();

        if (keys.contains(d->theme)) {
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

void CDecorator::removeThemeTemplate(const QString &key) {
    Q_D(CDecorator);

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

        // Notify related objects
        for (const auto &sub : qAsConst(tp.subscribers)) {
            sub->notifyAll();
        }
    }

    // Remove placeholder if there're no subscribers
    if (tp.subscribers.isEmpty() && tp.configs.isEmpty()) {
        delete it.value();
        d->themeTemplates.erase(it);
    }
}

void CDecorator::addThemeConfig(const QString &key, const QMap<QString, QStringList> &paths) {
    Q_D(CDecorator);

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
            if (it == map.begin()) {
                tp.dirtyThemeKeys.insert(themeKey);
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

        const auto &keys = tp.dirtyThemeKeys;
        tp.invalidate();

        if (keys.contains(d->theme)) {
            // Notify related objects
            for (const auto &sub : qAsConst(tp.subscribers)) {
                subscribersToUpdate.insert(sub);
            }
        }
    }

    for (const auto &sub : qAsConst(subscribersToUpdate)) {
        sub->notifyAll();
    }
}

void CDecorator::removeThemeConfig(const QString &key) {
    Q_D(CDecorator);

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

            auto &map = it3.value();
            auto it4 = map.find(key);

            // Save status
            bool beg = it4 == map.begin();
            map.erase(it4);
            if (map.isEmpty()) {
                map0.erase(it3);
            }

            // Remove placeholder if there're no subscribers
            if (tp.isEmpty()) {
                delete it2.value();
                d->themeTemplates.erase(it2);
            } else if (beg) {
                // Dirty if first
                tp.dirtyThemeKeys.insert(themeKey);
                templatesToUpdate.insert(&tp);
            }
        }
    }

    d->themeConfigs.erase(it);

    QSet<ThemeSubscriber *> subscribersToUpdate;
    for (auto p : qAsConst(templatesToUpdate)) {
        auto &tp = *p;

        const auto &keys = tp.dirtyThemeKeys;
        tp.invalidate();

        if (keys.contains(d->theme)) {
            // Notify related objects
            for (const auto &sub : qAsConst(tp.subscribers)) {
                subscribersToUpdate.insert(sub);
            }
        }
    }

    for (const auto &sub : qAsConst(subscribersToUpdate)) {
        sub->notifyAll();
    }
}

void CDecorator::installTheme(QWidget *w, const QStringList &templateKeys) {
    Q_D(CDecorator);

    if (d->themeSubscribers.contains(w)) {
        return;
    }

    ThemeSubscriber *ts;
    auto keySet = QSet<QString>(templateKeys.begin(), templateKeys.end());

    // Find group by key set
    auto it = d->themeSubscriberGroups.find(keySet);
    if (it == d->themeSubscriberGroups.end()) {
        // Add new
        ts = new ThemeSubscriber();
        d->themeSubscriberGroups.insert(keySet, ts);

        // Add to theme cache
        for (const auto &key : qAsConst(keySet)) {
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

    // Add guard to group
    auto tg = new ThemeGuard(w, this, d, ts);
    ts->widgets.insert(w, tg);

    // Detect destruction
    connect(w, &QObject::destroyed, this, &CDecorator::_q_themeSubscriberDestroyed);

    // Notify one forcefully
    tg->updateScreen();
}

void CDecorator::uninstallTheme(QWidget *w) {
    Q_D(CDecorator);

    auto it = d->themeSubscribers.find(w);
    if (it == d->themeSubscribers.end()) {
        return;
    }

    auto ts = it.value();
    {
        auto it1 = ts->widgets.find(w);
        Q_ASSERT(it1 != ts->widgets.end());

        // Remove guard from group
        auto tg = it1.value();
        delete tg;
        ts->widgets.erase(it1);

        // Remove from global set
        d->themeSubscribers.erase(it);
    }

    // Remove group if no widgets
    if (ts->widgets.isEmpty()) {
        auto keys = ts->templates.keys();
        auto keySet = QSet<QString>(keys.begin(), keys.end());

        // Remove from theme cache
        for (const auto &key : qAsConst(keySet)) {
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
        d->themeSubscriberGroups.remove(keySet); // Remove group
    }

    disconnect(w, &QObject::destroyed, this, &CDecorator::_q_themeSubscriberDestroyed);
}

// Protected or private members
CDecorator::CDecorator(CDecoratorPrivate &d, QObject *parent) : QsCoreDecorator(d, parent) {
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

void CDecorator::_q_themeSubscriberDestroyed() {
    auto w = qobject_cast<QWidget *>(sender());
    uninstallTheme(w);
}
