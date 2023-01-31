#ifndef QSIMPLEVAREXP_H
#define QSIMPLEVAREXP_H

#include <QJsonObject>

#include "QsGlobal.h"

class QSBASE_API QSimpleVarExp {
public:
    QSimpleVarExp();
    explicit QSimpleVarExp(const QString &pattern);
    ~QSimpleVarExp();

    QString Pattern;
    QHash<QString, QString> Variables;

public:
    void addJsonObject(const QJsonObject &obj);
    void addVariantMap(const QVariantMap &map);
    void addMap(const QMap<QString, QString> &map);
    void addHash(const QHash<QString, QString> &hash);
    void add(const QString &key, const QString &value);
    void clear();

    QString parse(const QString &exp) const;

public:
    static QHash<QString, QString> SystemValues();
};

#endif // QSIMPLEVAREXP_H