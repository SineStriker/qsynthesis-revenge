#ifndef QMRQFILE_H
#define QMRQFILE_H

#include <QDebug>
#include <QScopedPointer>

#include "qssvs_global.h"

/*
 *
 * Qt wrapper of mrq.h
 *
 */

class QSSVS_API QMrqFile {
public:
    QMrqFile();
    ~QMrqFile();

    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();

public:
    struct Entry {
        QString name;
        QList<double> f0;

        int nhop;
        int fs;
        qint32 timestamp; // read-only
        bool modified;
    };

    int version;
    QList<Entry> entries;
};

#endif // QMRQFILE_H
