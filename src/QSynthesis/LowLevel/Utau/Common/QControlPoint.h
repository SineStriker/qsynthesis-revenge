#ifndef Point_H
#define Point_H

#include "QUtauStd.h"

class Utau::Point {
public:
    double mX;
    double mY;
    Utau::PointType mP;

    Point();
    Point(const Point &oPoint);
    Point(const std::initializer_list<double> &list);

    Point(double oX, double oY);
    Point(double oX, double oY, PointType oP);

    //    Point &operator=(const Point &oPoint);

    Point operator+(const Point &oPoint) const;
    Point &operator+=(const Point &oPoint);

    Point operator-() const;
    Point operator-(const Point &oPoint) const;
    Point &operator-=(const Point &oPoint);

    bool operator==(const Point &oPoint) const;
    bool operator!=(const Point &oPoint) const;
    bool operator<(const Point &oPoint) const;

    friend QDebug operator<<(QDebug debug, const Point &oPoint);
};

typedef Utau::Point QControlPoint;

Q_DECLARE_METATYPE(QControlPoint)

#endif // Point_H
