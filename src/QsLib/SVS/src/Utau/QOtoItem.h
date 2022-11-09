#ifndef QOTOITEM_H
#define QOTOITEM_H

#include "Base/QUtaItem.h"

#include <QDebug>

class QSSVS_API QOtoItem : public QUtaItem {
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

    void reset() override;
    bool isEmpty() const override;

    bool operator<(const QOtoItem &item) const;
    bool operator==(const QOtoItem &item) const;
    bool operator!=(const QOtoItem &item) const;

    friend QDebug operator<<(QDebug debug, const QOtoItem &item);
};

#endif // QOTOITEM_H
