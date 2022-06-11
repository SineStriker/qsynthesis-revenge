#ifndef QOPENUTAUFILE_H
#define QOPENUTAUFILE_H

#include <QFile>

/*
 * Usage: util to parse or write OpenUtau project file(.ustx).
 */
class QOpenUtauFile {
public:
    QOpenUtauFile();
    ~QOpenUtauFile();

public:
    bool load(const QString &filename);
    bool save(const QString &filename);
};

#endif // QOPENUTAUFILE_H
