#ifndef QPREFIXMAP_H
#define QPREFIXMAP_H

#include <QMap>

#include "qssvs_global.h"
#include "qsutils_macros.h"

class QSSVS_API QPrefixMap {
    Q_CHARSET
public:
    QPrefixMap();
    ~QPrefixMap();

    QMap<int, QString> PrefixMap;
    QMap<int, QString> SuffixMap;

public:
    QString prefixedLyric(int noteNum, const QString &lyric) const;
    bool isEmpty() const;

protected:
    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();
};

#endif // QPREFIXMAP_H
