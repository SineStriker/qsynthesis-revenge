#include "CDecorator_p.h"
#include "ThemeGuard.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QApplication>
#include <QWidget>
#include <QWindow>

// LocaleData
static QTranslator *qmLoad(const QString &path) {
    auto t = new QTranslator();
    if (t->load(path)) {
        return t;
    }
    delete t;
    return nullptr;
}

LocaleData::LocaleData() {
}

LocaleData::~LocaleData() {
    uninstall();
}

void LocaleData::install(const QLocale &loc) {
    auto it = qmFiles.find(loc);
    if (it != qmFiles.end()) {
        install(it.value());
    } else {
        uninstall();
    }
}

void LocaleData::install(const QStringList &paths) {
    uninstall();
    for (const auto &path : paths) {
        auto t = qmLoad(path);
        if (t) {
            qApp->installTranslator(t);
            translators.append(t);
        }
    }
}

void LocaleData::uninstall() {
    if (translators.isEmpty() || qApp->isQuitLockEnabled()) {
        return;
    }

    for (auto it = translators.begin(); it != translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    translators.clear();
}

// ThemeSubscriber
ThemeSubscriber::ThemeSubscriber() : dirty(true) {
}

void ThemeSubscriber::notifyAll() {
    dirty = true;
    for (const auto &g : qAsConst(this->widgets)) {
        g->updateScreen();
    }
}


// ThemePlaceholder
bool ThemePlaceholder::isEmpty() const {
    if (!data.isNull()) {
        return false;
    }

    return subscribers.isEmpty() && configs.isEmpty();
}

void ThemePlaceholder::invalidate() {
    for (const auto &key : qAsConst(dirtyThemeKeys)) {
        auto it = configs.find(key);
        if (it == configs.end() || it.value().data.isEmpty() || data.isNull()) {
            stylesheetCaches.remove(key);
            continue;
        }

        auto &map0 = it.value().data;
        auto &map = map0.begin().value();

        QMap<QString, QColor> colors;
        QMap<QString, int> sizes;
        for (const auto &conf : qAsConst(map)) {
            auto it2 = conf->colors.find(ns);
            if (it2 != conf->colors.end()) {
                const auto &map3 = it2.value();
                for (auto it4 = map3.begin(); it4 != map3.end(); ++it4) {
                    colors.insert(it4.key(), it4.value());
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
        stylesheetCaches[key] = data->parse(colors, sizes);
    }
    dirtyThemeKeys.clear();
}


// CDecoratorPrivate

CDecoratorPrivate::CDecoratorPrivate() {
}

CDecoratorPrivate::~CDecoratorPrivate() {
}

void CDecoratorPrivate::init() {
    Q_Q(CDecorator);

    theme = "Zhibin - Dark";
    loc = QLocale::system();

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