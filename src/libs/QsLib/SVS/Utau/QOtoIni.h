#ifndef QOTOINI_H
#define QOTOINI_H

#include "Base/QUtaFile.h"

#include "QOtoItem.h"

class QSSVS_API QOtoIni : public QUtaFile {
    Q_CHARSET
public:
    QOtoIni();
    QOtoIni(const QString &dirname);
    ~QOtoIni();

    QMap<QString, QList<QOtoItem>> OtoSamples;

public:
    bool load(const QString &filename) override;
    bool save(const QString &filename) override;

    void reset() override;
    bool isEmpty() const override;
};

#endif // QOTOINI_H
