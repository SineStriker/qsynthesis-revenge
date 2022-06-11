#include "QControlPoint.h"

using namespace Utau;

Point::Point() {
    mX = 0.0;
    mY = 0.0;
    mP = sJoin;
}

Point::Point(const Point &oPoint) {
    mX = oPoint.mX;
    mY = oPoint.mY;
    mP = oPoint.mP;
}

Point::Point(const std::initializer_list<double> &list) {
    Q_ASSERT(list.size() == 2);
    mX = *list.begin();
    mY = *std::next(list.begin());
    mP = sJoin;
}

Point::Point(double oX, double oY) {
    mX = oX;
    mY = oY;
    mP = sJoin;
}

Point::Point(double oX, double oY, PointType oP) {
    mX = oX;
    mY = oY;
    mP = oP;
}

// Point &Point::operator=(const Point &oPoint) {
//    if (this != &oPoint) {
//        mX = oPoint.mX;
//        mY = oPoint.mY;
//        mP = oPoint.mP;
//    }
//    return *this;
//}

Point Point::operator+(const Point &oPoint) const {
    Point aResult;
    aResult.mX = mX + oPoint.mX;
    aResult.mY = mY + oPoint.mY;

    return aResult;
}

Point &Point::operator+=(const Point &oPoint) {
    mX += oPoint.mX;
    mY += oPoint.mY;

    return *this;
}

Point Point::operator-() const {
    Point aResult(-mX, -mY, mP);
    return aResult;
}

Point Point::operator-(const Point &oPoint) const {
    Point aResult;
    aResult.mX = mX - oPoint.mX;
    aResult.mY = mY - oPoint.mY;

    return aResult;
}

Point &Point::operator-=(const Point &oPoint) {
    mX -= oPoint.mX;
    mY -= oPoint.mY;

    return *this;
}

bool Point::operator==(const Point &oPoint) const {
    return (oPoint.mX == mX && oPoint.mY == mY && oPoint.mP == mP);
}

bool Point::operator!=(const Point &oPoint) const {
    return !((*this) == oPoint);
}

bool Point::operator<(const Point &oPoint) const {
    return mX < oPoint.mX;
}

QDebug Utau::operator<<(QDebug debug, const Point &oPoint) {
    QString info =
        QString("Point(mX:%1; mY:%2; mP:%3)").arg(oPoint.mX).arg(oPoint.mY).arg(oPoint.mP);
    debug.noquote() << info;
    return debug;
}
