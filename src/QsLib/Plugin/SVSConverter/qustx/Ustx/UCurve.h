#ifndef UCURVE_H
#define UCURVE_H

#include <QSharedPointer>
#include <QStringList>

#include "UExpression.h"

/**
 *  Interpret from OpenUtau.Core.Ustx
 */

class UCurve {
public:
    UCurve();
    UCurve(UExpressionDescriptor *descriptor);
    UCurve(const QString &abbr);
    UCurve(const UCurve &another);
    UCurve(UCurve &&another);
    ~UCurve();

    UCurve &operator=(const UCurve &another);
    UCurve &operator=(UCurve &&another);

    bool isEmpty() const;
    bool isEmptyBetween(int x0, int x1, int defaultValue) const;

    int sample(int x) const;

    void set(int x, int y, int lastX, int lastY);

    void simplify();
    void simplify(int first, int last, double tolerance, QList<int> &toKeep);

private:
    void insert(int x, int y);
    void deleteBetweenExclusive(int x1, int x2);
    double perpendicularDistance(int x, int y, int x1, int y1, int x2, int y2);

private:
    QSharedPointer<UExpressionDescriptor> descriptor;

public:
    QList<int> xs;
    QList<int> ys;
    QString abbr;

    const int interval = 5;
};

#endif // UCURVE_H
