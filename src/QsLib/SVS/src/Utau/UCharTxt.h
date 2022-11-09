#ifndef UCHARTXT_H
#define UCHARTXT_H

#include "Base/QUtaFile.h"

class QSSVS_API UCharTxt : public QUtaFile {
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
    bool load(const QString &filename) override;
    bool save(const QString &filename) override;

    void reset() override;
    bool isEmpty() const override;
};

#endif // UCHARTXT_H
