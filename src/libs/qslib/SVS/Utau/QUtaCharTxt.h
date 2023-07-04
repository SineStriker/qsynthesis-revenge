#ifndef QUTACHARTXT_H
#define QUTACHARTXT_H

#include "Base/QUtaFile.h"

class QSSVS_API QUtaCharTxt : public QUtaFile {
    Q_CHARSET
public:
    QUtaCharTxt();
    ~QUtaCharTxt();

    QString name;
    QString author;
    QString avatar;
    QString sprite;

    QStringList custom; // Other information which can't be edited

public:
    bool load(const QString &filename) override;
    bool save(const QString &filename) override;

    void reset() override;
    bool isEmpty() const override;
};

#endif // QUTACHARTXT_H
