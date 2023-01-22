#include "LocalLinguist.h"

#include "CDecorator.h"

#include <QCoreApplication>
#include <QDebug>

class LocalLinguistPrivate {
public:
    LocalLinguist *q_ptr;

    // Translators
    bool translate(const QString &filename) {
        QTranslator *t = new QTranslator(q_ptr);

        if (t->load(filename)) {
            addTranslation(t);
            return true;
        }
        delete t;

        return false;
    }

    void eliminate() {
        if (qApp->isQuitLockEnabled()) {
            return;
        }
        for (auto it = translators.begin(); it != translators.end(); ++it) {
            auto t = *it;
            qApp->removeTranslator(t);
            delete t;
        }
        translators.clear();
    }

    bool translateOnly(const QString &filename, bool dryRun = false) {
        QTranslator *t = new QTranslator(q_ptr);
        if (!dryRun) {
            eliminate();
        }
        if (t->load(filename)) {
            if (dryRun) {
                eliminate();
            }
            addTranslation(t);
            return true;
        }
        delete t;

        return false;
    }

    void addTranslation(QTranslator *t) {
        qApp->installTranslator(t);
        translators.insert(t);
    }

    QSet<QTranslator *> translators;
    QHash<int, QStringList> qmPaths;
};

LocalLinguist::LocalLinguist(QObject *parent) : QObject(parent), d_ptr(new LocalLinguistPrivate()) {
    d_ptr->q_ptr = this;

    Q_TR_NOTIFY(LocalLinguist);
}

LocalLinguist::~LocalLinguist() {
    d_ptr->eliminate();
}

void LocalLinguist::reloadStrings(int locale, const QString &key) {
    auto &d = *d_ptr.data();
    d.eliminate();

    auto it = d.qmPaths.find(locale);
    if (it != d.qmPaths.end()) {
        QStringList paths = it.value();
        for (const QString &path : qAsConst(paths)) {
            d.translate(path);
        }
    }
}

void LocalLinguist::addLocale(int locale, const QStringList &paths) {
    d_ptr->qmPaths.insert(locale, paths);
}

void LocalLinguist::removeLocale(int locale) {
    d_ptr->qmPaths.remove(locale);
}
