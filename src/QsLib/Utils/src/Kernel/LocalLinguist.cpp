#include "LocalLinguist.h"

#include <QCoreApplication>

LocalLinguist::LocalLinguist(QObject *parent) : QObject(parent) {
}

LocalLinguist::~LocalLinguist() {
    eliminate();
}

bool LocalLinguist::translate(const QString &filename) {
    QTranslator *t = new QTranslator(this);

    if (t->load(filename)) {
        addTranslation(t);
        return true;
    }
    delete t;

    return false;
}

void LocalLinguist::eliminate() {
    for (auto it = translators.begin(); it != translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    translators.clear();
}

bool LocalLinguist::translateOnly(const QString &filename, bool dryRun) {
    QTranslator *t = new QTranslator(this);
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

void LocalLinguist::addTranslation(QTranslator *t) {
    qApp->installTranslator(t);
    translators.insert(t);
}
