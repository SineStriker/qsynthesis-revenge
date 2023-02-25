#ifndef QUTAFILE_H
#define QUTAFILE_H

#include "QUtaItem.h"

class QSSVS_API QUtaFile : public QUtaItem {
public:
    QUtaFile();
    virtual ~QUtaFile();

public:
    virtual bool load(const QString &filename);
    virtual bool save(const QString &filename);

    bool isEmpty() const override;
};

#endif // QUTAFILE_H
