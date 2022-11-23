#ifndef __QNRBFFILE_H__
#define __QNRBFFILE_H__

#include "QNrbfRecord.h"

class QSUTILS_API QNrbfFile {
public:
    QNrbfFile();
    ~QNrbfFile();

    bool load(const QString &filename);
    bool save(const QString &filename);

public:
    QVector<QNrbfRecord> records;
};

#endif // __QNRBFFILE_H__
