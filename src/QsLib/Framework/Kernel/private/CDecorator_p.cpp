#include "CDecorator_p.h"

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

void LocaleData::install(int loc) {
    uninstall();

    auto it = qmFiles.find(loc);
    if (it != qmFiles.end()) {
        for (const auto &path : it.value()) {
            auto t = qmLoad(path);
            if (t) {
                qApp->installTranslator(t);
                translators.append(t);
            }
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


// CDecoratorPrivate

CDecoratorPrivate::CDecoratorPrivate() {
}

CDecoratorPrivate::~CDecoratorPrivate() {
}

void CDecoratorPrivate::init() {
    Q_Q(CDecorator);

    theme = CDecorator::Dark;
    loc = CDecorator::UnitedStates;

    themeMax = CDecorator::UserTheme;

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