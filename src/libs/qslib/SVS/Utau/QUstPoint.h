#ifndef QUSTPOINT_H
#define QUSTPOINT_H

#include "Base/QUtaItem.h"

#include <QDebug>

class QSSVS_API QUstPoint : public QUtaItem {
public:
    enum PointType { sJoin, linearJoin, rJoin, jJoin };
    double X;
    double Y;
    PointType P;

public:
    QUstPoint();
    QUstPoint(const std::initializer_list<double> &list);

    QUstPoint(double oX, double oY);
    QUstPoint(double oX, double oY, PointType oP);

    void reset() override;
    bool isEmpty() const override;

    QUstPoint operator+(const QUstPoint &point) const;
    QUstPoint &operator+=(const QUstPoint &point);

    QUstPoint operator-() const;
    QUstPoint operator-(const QUstPoint &point) const;
    QUstPoint &operator-=(const QUstPoint &point);

    bool operator==(const QUstPoint &point) const;
    bool operator!=(const QUstPoint &point) const;
    bool operator<(const QUstPoint &point) const;

    QSSVS_API friend QDebug operator<<(QDebug debug, const QUstPoint &point);
};

#endif // QUSTPOINT_H
