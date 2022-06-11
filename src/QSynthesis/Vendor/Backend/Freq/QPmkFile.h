#ifndef QPMKFILE_H
#define QPMKFILE_H

/*
 * I/O Device for frequency map of TIPS resampler
 * Copyright: scientistB
 * Reference: Masao
 */

#include <QFile>

class QPmkFile {
public:
    QPmkFile();
    ~QPmkFile();

    void reset();

    bool load(const QString &filename);
    bool save(const QString &filename);

    double KeyFrequency;
    QList<double> Frequency;

    const double EMPTY_FREQUENCY = 55.0; // empty frequency in frq file is 55.0
};

#endif // QPMKFILE_H
