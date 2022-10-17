#ifndef QUSTPOINT_H
#define QUSTPOINT_H

#include <QDebug>

#include "qssvs_global.h"

class QSSVS_API QUstPoint {
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

    QUstPoint operator+(const QUstPoint &point) const;
    QUstPoint &operator+=(const QUstPoint &point);

    QUstPoint operator-() const;
    QUstPoint operator-(const QUstPoint &point) const;
    QUstPoint &operator-=(const QUstPoint &point);

    bool operator==(const QUstPoint &point) const;
    bool operator!=(const QUstPoint &point) const;
    bool operator<(const QUstPoint &point) const;

    friend QDebug operator<<(QDebug debug, const QUstPoint &point);
};

#endif // QUSTPOINT_H
