#include "QMCoreDecorator_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QLocale>

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

void LocaleData::install(const QString &loc) {
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
    if (translators.isEmpty() || qApp->closingDown()) {
        return;
    }

    for (auto it = translators.begin(); it != translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    translators.clear();
}

QMCoreDecoratorPrivate::QMCoreDecoratorPrivate() {
}

QMCoreDecoratorPrivate::~QMCoreDecoratorPrivate() {
}

void QMCoreDecoratorPrivate::init() {
    loc = QLocale::system().name();
    // loc = "zh_CN";
}

void QMCoreDecoratorPrivate::_q_localeSubscriberDestroyed() {
    Q_Q(QMCoreDecorator);
    q->uninstallLocale(sender());
}
