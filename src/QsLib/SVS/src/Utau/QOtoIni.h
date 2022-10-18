#ifndef QOTOINI_H
#define QOTOINI_H

#include "QOtoItem.h"

#include "qsutils_macros.h"

class QSSVS_API QOtoIni  {
    Q_CHARSET
public:
    QOtoIni();
    QOtoIni(const QString &dirname);
    ~QOtoIni();

    QMap<QString, QList<QOtoItem>> OtoSamples;

public:
    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();
};

#endif // QOTOINI_H
