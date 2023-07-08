#ifndef QOTOITEMBRIEF_H
#define QOTOITEMBRIEF_H

#include "Base/QUtaItem.h"

#include <QDebug>

class QSSVS_API QOtoItemBrief : public QUtaItem {
public:
    double PreUtterance;
    double VoiceOverlap;
    double StartPoint;

    QOtoItemBrief();
    QOtoItemBrief(double pre, double ove, double stp);
    QOtoItemBrief(const std::initializer_list<double> &list);

    void reset() override;
    bool isEmpty() const override;

    bool operator==(const QOtoItemBrief &oCorrect) const;
    bool operator!=(const QOtoItemBrief &oCorrect) const;

    QSSVS_API friend QDebug operator<<(QDebug debug, const QOtoItemBrief &item);
};

#endif // QOTOITEMBRIEF_H
