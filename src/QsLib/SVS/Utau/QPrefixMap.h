#ifndef QPREFIXMAP_H
#define QPREFIXMAP_H

#include "Base/QUtaFile.h"

#include <QMap>

class QSSVS_API QPrefixMap : public QUtaFile {
    Q_CHARSET
public:
    QPrefixMap();
    ~QPrefixMap();

    QMap<int, QString> PrefixMap;
    QMap<int, QString> SuffixMap;

    QString prefixedLyric(int noteNum, const QString &lyric) const;

protected:
    bool load(const QString &filename) override;
    bool save(const QString &filename) override;

    void reset() override;
    bool isEmpty() const override;
};

#endif // QPREFIXMAP_H
