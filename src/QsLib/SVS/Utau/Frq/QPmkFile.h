#ifndef QPMKFILE_H
#define QPMKFILE_H

/*
 * I/O Device for frequency map of TIPS resampler
 * Copyright: scientistB
 * Reference: Masao
 */

#include <QList>

#include "QsSVSGlobal.h"

class QSSVS_API QPmkFile {
public:
    QPmkFile();
    ~QPmkFile();

    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();

    double KeyFrequency;
    QList<double> Frequency;

    const double EMPTY_FREQUENCY = 55.0; // empty frequency in frq file is 55.0
};

#endif // QPMKFILE_H
