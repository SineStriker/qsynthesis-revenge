#ifndef QMSIMPLEVAREXP_H
#define QMSIMPLEVAREXP_H

#include <QJsonObject>

#include "QMGlobal.h"

class QMCORE_EXPORT QMSimpleVarExp {
public:
    QMSimpleVarExp();
    explicit QMSimpleVarExp(const QString &pattern);
    ~QMSimpleVarExp();

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

    static QString EvaluateVariables(const QString &s, const QHash<QString, QString> &dict, const QString &pattern = {},
                                     bool recursive = false);
};

#endif // QMSIMPLEVAREXP_H