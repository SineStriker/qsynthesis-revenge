#ifndef LOCALLINGUIST_H
#define LOCALLINGUIST_H

#include <QSet>
#include <QTranslator>

#include "qsutils_global.h"

class QSUTILS_API LocalLinguist : public QObject {
    Q_OBJECT
public:
    explicit LocalLinguist(QObject *parent = nullptr);
    ~LocalLinguist();

    // Translators
    bool translate(const QString &filename);
    void eliminate();

    bool translateOnly(const QString &filename, bool dryRun = false);

    QSet<QTranslator *> translators;

private:
    void addTranslation(QTranslator *t);
};

#endif // LOCALLINGUIST_H
