#ifndef LOCALLINGUIST_H
#define LOCALLINGUIST_H

#include <QHash>
#include <QSet>
#include <QTranslator>

#include "QsFrameworkGlobal.h"

class LocalLinguistPrivate;

class QSFRAMEWORK_API LocalLinguist : public QObject {
    Q_OBJECT
public:
    explicit LocalLinguist(QObject *parent = nullptr);
    ~LocalLinguist();

    void reloadStrings(int locale, const QString &key);

public:
    void addLocale(int locale, const QStringList &paths);
    void removeLocale(int locale);

protected:
    QScopedPointer<LocalLinguistPrivate> d_ptr;

    friend class LocalLinguistPrivate;
};

#endif // LOCALLINGUIST_H
