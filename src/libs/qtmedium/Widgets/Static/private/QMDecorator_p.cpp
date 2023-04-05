#include "QMDecorator_p.h"
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
            QMDecoratorPrivate::subscriberUpdateEnqueue(tg);
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
    QMDecoratorPrivate::subscriberUpdateEnqueue(tg);
}

void ThemeSubscriber::removeWidget(QWidget *w) {
    auto it = widgets.find(w);
    if (it == widgets.end()) {
        return;
    }
    auto tg = it.value();
    uninstallScreen(tg);
    if (tg->queueIterator != QMDecoratorPrivate::subscriberUpdateQueue.end()) {
        qDebug() << "ThemeSubscriber: subscriber" << tg << "has been removed when it's in update queue";
        QMDecoratorPrivate::subscriberUpdateQueue.erase(tg->queueIterator);
        tg->queueIterator = QMDecoratorPrivate::subscriberUpdateQueue.end();
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


// QMDecoratorPrivate

QMDecoratorPrivate::QMDecoratorPrivate() {
    self = this;
}

QMDecoratorPrivate::~QMDecoratorPrivate() {
}

void QMDecoratorPrivate::init() {
    Q_Q(QMDecorator);

    theme = "Zhibin - Dark";

    // Notify screen add or remove
    connect(qApp, &QApplication::screenAdded, this, &QMDecoratorPrivate::_q_screenAdded);
    connect(qApp, &QApplication::screenRemoved, this, &QMDecoratorPrivate::_q_screenRemoved);

    // Monitor all screen at first time
    auto screens = QApplication::screens();
    for (auto screen : qAsConst(screens)) {
        connect(screen, &QScreen::physicalDotsPerInchChanged, this, &QMDecoratorPrivate::_q_deviceRatioChanged);
        connect(screen, &QScreen::logicalDotsPerInchChanged, this, &QMDecoratorPrivate::_q_logicalRatioChanged);
    }
}

void QMDecoratorPrivate::screenChange_helper(QScreen *screen) {
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
            QMDecoratorPrivate::subscriberUpdateEnqueue(tg);
        }
    }
}

void QMDecoratorPrivate::subscriberUpdateEnqueue(ThemeGuard *tg) {
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

QMDecoratorPrivate *QMDecoratorPrivate::self = nullptr;

std::list<ThemeGuard *> QMDecoratorPrivate::subscriberUpdateQueue;

void QMDecoratorPrivate::_q_screenAdded(QScreen *screen) {
    connect(screen, &QScreen::physicalDotsPerInchChanged, this, &QMDecoratorPrivate::_q_deviceRatioChanged);
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &QMDecoratorPrivate::_q_logicalRatioChanged);
}

void QMDecoratorPrivate::_q_screenRemoved(QScreen *screen) {
    disconnect(screen, &QScreen::physicalDotsPerInchChanged, this, &QMDecoratorPrivate::_q_deviceRatioChanged);
    disconnect(screen, &QScreen::logicalDotsPerInchChanged, this, &QMDecoratorPrivate::_q_logicalRatioChanged);

    // How to deal with windows on the removed screen?
}

void QMDecoratorPrivate::_q_deviceRatioChanged(double dpi) {
    Q_Q(QMDecorator);

    auto screen = qobject_cast<QScreen *>(sender());
    screenChange_helper(screen);
    emit q->deviceRatioChanged(screen, dpi);
}

void QMDecoratorPrivate::_q_logicalRatioChanged(double dpi) {
    Q_Q(QMDecorator);

    auto screen = qobject_cast<QScreen *>(sender());
    screenChange_helper(screen);
    emit q->logicalRatioChanged(screen, dpi);
}

void QMDecoratorPrivate::_q_themeSubscriberDestroyed() {
    Q_Q(QMDecorator);
    auto w = qobject_cast<QWidget *>(sender());
    q->uninstallTheme(w);
}
