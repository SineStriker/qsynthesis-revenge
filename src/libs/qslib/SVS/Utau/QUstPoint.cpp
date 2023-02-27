#include "QUstPoint.h"

#include "Utils/QUtaUtils.h"

QUstPoint::QUstPoint() {
    QUstPoint::reset();
}

QUstPoint::QUstPoint(const std::initializer_list<double> &list) {
    Q_ASSERT(list.size() == 2);
    X = *list.begin();
    Y = *std::next(list.begin());
    P = sJoin;
}

QUstPoint::QUstPoint(double oX, double oY) {
    X = oX;
    Y = oY;
    P = sJoin;
}

QUstPoint::QUstPoint(double oX, double oY, PointType oP) {
    X = oX;
    Y = oY;
    P = oP;
}

void QUstPoint::reset() {
    X = 0.0;
    Y = 0.0;
    P = sJoin;
}

bool QUstPoint::isEmpty() const {
    return (X == 0) && (Y == 0);
}

QUstPoint QUstPoint::operator+(const QUstPoint &point) const {
    QUstPoint res;
    res.X = X + point.X;
    res.Y = Y + point.Y;
    return res;
}

QUstPoint &QUstPoint::operator+=(const QUstPoint &point) {
    X += point.X;
    Y += point.Y;

    return *this;
}

QUstPoint QUstPoint::operator-() const {
    return QUstPoint(-X, -Y, P);
}

QUstPoint QUstPoint::operator-(const QUstPoint &point) const {
    QUstPoint res;
    res.X = X - point.X;
    res.Y = Y - point.Y;
    return res;
}

QUstPoint &QUstPoint::operator-=(const QUstPoint &point) {
    X -= point.X;
    Y -= point.Y;
    return *this;
}

bool QUstPoint::operator==(const QUstPoint &point) const {
    return (point.X == X && point.Y == Y && point.P == P);
}

bool QUstPoint::operator!=(const QUstPoint &point) const {
    return !((*this) == point);
}

bool QUstPoint::operator<(const QUstPoint &point) const {
    return X < point.X;
}

QDebug operator<<(QDebug debug, const QUstPoint &point) {
    QString info = QString("QUstPoint(X:%1; Y:%2; P:%3)")
                       .arg(point.X)
                       .arg(point.Y)
                       .arg(QUtaUtils::PointToString(point.P));
    debug.noquote() << info;
    return debug;
}
