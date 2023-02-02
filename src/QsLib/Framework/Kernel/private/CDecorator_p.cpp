#include "CDecorator_p.h"
#include "ThemeGuard.h"

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
            tg->updateScreen();
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
    tg->updateScreen();
}

void ThemeSubscriber::removeWidget(QWidget *w) {
    auto it = widgets.find(w);
    if (it == widgets.end()) {
        return;
    }
    auto tg = it.value();
    uninstallScreen(tg);
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
    {
        auto it = stylesheetCaches.find(screen);
        if (it != stylesheetCaches.end()) {
            return it.value();
        }
    }

    auto it = configs.find(qIDec->theme());
    if (it == configs.end() || it.value().data.isEmpty() || data.isNull()) {
        return QString();
    }

    auto &map0 = it.value().data;
    auto &map = map0.begin().value();

    QMap<QString, QString> strs;
    QMap<QString, int> sizes;
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

    QString stylesheet = data->parse(strs, sizes, screen->logicalDotsPerInch());
    stylesheetCaches[screen] = stylesheet;
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
            tg->updateScreen();
        }
    }
}

CDecoratorPrivate *CDecoratorPrivate::self = nullptr;
