#ifndef QOTOITEM_H
#define QOTOITEM_H

#include <QDebug>

#include "qssvs_global.h"

class QSSVS_API QOtoItem {
public:
    QString FileName;
    QString Alias;
    double Offset;
    double Constant; // Also signed as Consonant
    double Blank;
    double PreUtterance;
    double VoiceOverlap;

    QOtoItem();
    QOtoItem(const QString &filename);

    void clear();
    bool isEmpty() const;

    bool operator<(const QOtoItem &item) const;
    bool operator==(const QOtoItem &item) const;
    bool operator!=(const QOtoItem &item) const;

    friend QDebug operator<<(QDebug debug, const QOtoItem &item);
};

#endif // QOTOITEM_H
