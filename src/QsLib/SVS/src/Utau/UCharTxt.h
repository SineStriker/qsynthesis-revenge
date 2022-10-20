#ifndef UCHARTXT_H
#define UCHARTXT_H

#include <QStringList>

#include "qssvs_global.h"
#include "qsutils_macros.h"

class QSSVS_API UCharTxt {
    Q_CHARSET
public:
    UCharTxt();
    ~UCharTxt();

    QString name;
    QString author;
    QString avatar;
    QString sprite;

    QStringList custom; // Other information which can't be edited

public:
    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();
};

#endif // UCHARTXT_H
