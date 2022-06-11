#ifndef QFRQFILE_H
#define QFRQFILE_H

/*
 * This file is the Qt version of frq_reader.py
 * Reference: https://github.com/titinko/frq_reader
 * Copyright 2018 titinko
 */

#include <QDebug>
#include <QFile>

class QFrqFile {
public:
    QFrqFile();
    ~QFrqFile();
    void reset();

    bool load(const QString &filename);
    bool save(const QString &filename);

    QList<double> Amplitude;
    QList<double> Frequency;

    unsigned SamplesPerFrq;
    double AverageFrq;
};

#endif // QFRQFILE_H
