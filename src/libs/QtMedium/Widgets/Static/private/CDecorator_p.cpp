#include "CDecorator_p.h"
#include "ThemeGuard.h"

#include "QMWidgetsHost.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QApplication>
#include <QScreen>
#include <QWidget>
#include <QWindow>

// ThemeSubscriber

ScreenSet::ScreenSet(QScreen *screen) : dirty(true), screen(screen) {
}

ThemeSubscriber::ThemeSubscriber() {
}

void ThemeSubscriber::notifyAll() {
    // Set dirty
    for (auto it = screens.begin(); it != screens.end(); ++it) {
        it.value()->dirty = true;
    }

    // Update all
    for (auto it = screens.begin(); it != screens.end(); ++it) {
        for (const auto &tg : qAsConst(it.value()->widgets)) {
            CDecoratorPrivate::subscriberUpdateEnqueue(tg);
        }
    }
}

void ThemeSubscriber::installScreen(ThemeGuard *tg) {
    auto screen = tg->w->screen();
    auto it = screens.find(screen);

    ScreenSet *ss;
    if (it == screens.end()) {
        ss = new ScreenSet(screen);
        screens.insert(screen, ss);
    } else {
        ss = it.value();
    }
    ss->widgets.insert(tg->w, tg);

    tg->screenSet = ss;
}

void ThemeSubscriber::uninstallScreen(ThemeGuard *tg) {
    auto screen = tg->screenSet->screen;

    auto it = screens.find(screen);
    if (it == screens.end()) {
        return;
    }

    auto ss = it.value();
    ss->widgets.remove(tg->w);

    if (ss->widgets.isEmpty()) {
        delete ss;
        screens.erase(it);
    }

    tg->screenSet = nullptr;
}

void ThemeSubscriber::switchScreen(ThemeGuard *tg) {
    uninstallScreen(tg);
    installScreen(tg);
}

bool ThemeSubscriber::isEmpty() const {
    return widgets.isEmpty();
}

void ThemeSubscriber::addWidget(QWidget *w) {
    auto tg = new ThemeGuard(w, this);
    widgets.insert(w, tg);
    installScreen(tg);

    // Notify forcefully
    CDecoratorPrivate::subscriberUpdateEnqueue(tg);
}

void ThemeSubscriber::removeWidget(QWidget *w) {
    auto it = widgets.find(w);
    if (it == widgets.end()) {
        return;
    }
    auto tg = it.value();
    uninstallScreen(tg);
    if (tg->queueIterator != CDecoratorPrivate::subscriberUpdateQueue.end()) {
        qDebug() << "ThemeSubscriber: subscriber" << tg
                 << "has been removed when it's in update queue";
        CDecoratorPrivate::subscriberUpdateQueue.erase(tg->queueIterator);
        tg->queueIterator = CDecoratorPrivate::subscriberUpdateQueue.end();
    }
    delete tg;

    widgets.erase(it);
}


// ThemePlaceholder
bool ThemePlaceholder::isEmpty() const {
    if (!data.isNull()) {
        return false;
    }

    return subscribers.isEmpty() && configs.isEmpty();
}

void ThemePlaceholder::invalidate() {
    stylesheetCaches.clear();
}

QString ThemePlaceholder::getAndCache(QScreen *screen) {
    if (qmHost->isAboutToQuit()) {
        return QString();
    }

    {
        auto it = stylesheetCaches.find(screen);
        if (it != stylesheetCaches.end()) {
            return it.value();
        }
    }

    auto it = configs.find(qIDec->theme());
    if (it == configs.end() || it.value().data.empty() || data.isNull()) {
        return QString();
    }

    QMap<QString, ThemeConfig::Value> values;

    auto &map0 = it.value().data;
#if !ENABLE_PARTIAL_OVERRIDE
    auto &map = map0.begin().value();
    for (const auto &conf : qAsConst(map)) {
        auto it2 = conf->strs.find(ns);
        if (it2 != conf->strs.end()) {
            const auto &map3 = it2.value();
            for (auto it4 = map3.begin(); it4 != map3.end(); ++it4) {
                strs.insert(it4.key(), it4.value());
            }
        }
        auto it3 = conf->sizes.find(ns);
        if (it3 != conf->sizes.end()) {
            const auto &map3 = it3.value();
            for (auto it4 = map3.begin(); it4 != map3.end(); ++it4) {
                sizes.insert(it4.key(), it4.value());
            }
        }
    }
#else
    for (auto it = map0.rbegin(); it != map0.rend(); it++) {
        const auto &map = it->second;
        for (const auto &conf : qAsConst(map)) {
            auto it2 = conf->values.find(ns);
            if (it2 != conf->values.end()) {
                const auto &map3 = it2.value();
                for (auto it4 = map3.begin(); it4 != map3.end(); ++it4) {
                    values.insert(it4.key(), it4.value());
                }
            }
        }
    }
#endif

    QString stylesheet = data->parse(values, screen->logicalDotsPerInch());
    stylesheetCaches[screen] = stylesheet;
#ifdef CONFIG_ENABLE_DEBUG
    // qDebug().noquote() << stylesheet;
#endif
    return stylesheet;
}


// CDecoratorPrivate

CDecoratorPrivate::CDecoratorPrivate() {
    self = this;
}

CDecoratorPrivate::~CDecoratorPrivate() {
}

void CDecoratorPrivate::init() {
    Q_Q(CDecorator);

    theme = "Zhibin - Dark";

    // Notify screen add or remove
    q->connect(qApp, &QApplication::screenAdded, q, &CDecorator::_q_screenAdded);
    q->connect(qApp, &QApplication::screenRemoved, q, &CDecorator::_q_screenRemoved);

    // Monitor all screen at first time
    auto screens = qApp->screens();
    for (auto screen : qAsConst(screens)) {
        q->connect(screen, &QScreen::physicalDotsPerInchChanged, q,
                   &CDecorator::_q_deviceRatioChanged);
        q->connect(screen, &QScreen::logicalDotsPerInchChanged, q,
                   &CDecorator::_q_logicalRatioChanged);
    }
}

void CDecoratorPrivate::screenChange_helper(QScreen *screen) {
    for (auto tp : qAsConst(themeTemplates)) {
        tp->stylesheetCaches.remove(screen);
    }

    for (auto ts : qAsConst(themeSubscriberGroups)) {
        auto it = ts->screens.find(screen);
        if (it == ts->screens.end()) {
            continue;
        }

        // Notify all
        for (auto tg : qAsConst(it.value()->widgets)) {
            CDecoratorPrivate::subscriberUpdateEnqueue(tg);
        }
    }
}

void CDecoratorPrivate::subscriberUpdateEnqueue(ThemeGuard *tg) {
    // Update waiting subscribers
    while (!subscriberUpdateQueue.empty()) {
        auto first = subscriberUpdateQueue.front();
        if (!first->updateScreen()) {
            break;
        }
        first->queueIterator = subscriberUpdateQueue.end();
        subscriberUpdateQueue.pop_front();
    }
    if (tg) {
        if (!subscriberUpdateQueue.empty() || !tg->updateScreen()) {
            tg->queueIterator = subscriberUpdateQueue.insert(subscriberUpdateQueue.end(), tg);
        }
    }
}

CDecoratorPrivate *CDecoratorPrivate::self = nullptr;

std::list<ThemeGuard *> CDecoratorPrivate::subscriberUpdateQueue;
