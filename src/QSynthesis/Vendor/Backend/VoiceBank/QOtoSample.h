#ifndef QOTOSAMPLE_H
#define QOTOSAMPLE_H

#include "QUtauBasic.h"

class QOtoSample : public QList<QGenonSettings> {
public:
    QOtoSample();
    QOtoSample(const QString &filename);
    ~QOtoSample();

public:
    QString filename() const;
    void setFilename(const QString &filename);

    void clearAll();

    bool valid() const;
    bool frqExist() const;
    bool mrqExist() const;

public:
    bool operator==(const QOtoSample &sample) const;
    bool operator!=(const QOtoSample &sample) const;
    bool operator<(const QOtoSample &sample) const;

    friend uint qHash(const QOtoSample &samples, uint seed);

protected:
    QString m_filename;
};

#endif // QOTOSAMPLE_H
