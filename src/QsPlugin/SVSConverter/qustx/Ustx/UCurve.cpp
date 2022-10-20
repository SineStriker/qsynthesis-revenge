#include "UCurve.h"

#include "Util/MusicMath.h"

#include <QtMath>

#define binarySearch(ARR, I) (std::lower_bound(ARR.begin(), ARR.end(), I) - ARR.begin());
#define removeRange(ARR, INDEX, LEN) ARR.erase(ARR.begin() + (INDEX), ARR.begin() + (INDEX) + (LEN))

static bool allZero(const QList<int> &list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (*it != 0) {
            return false;
        }
    }
    return true;
}

UCurve::UCurve() {
}

UCurve::UCurve(UExpressionDescriptor *descriptor) : descriptor(descriptor), abbr(descriptor->abbr) {
}

UCurve::UCurve(const QString &abbr) : abbr(abbr) {
}

UCurve::UCurve(const UCurve &another) {
    descriptor = another.descriptor;
    xs = another.xs;
    ys = another.ys;
    abbr = another.abbr;
}

UCurve::UCurve(UCurve &&another) {
    descriptor = std::move(another.descriptor);
    xs = another.xs;
    ys = another.ys;
    abbr = another.abbr;
}

UCurve::~UCurve() {
}

UCurve &UCurve::operator=(const UCurve &another) {
    descriptor = another.descriptor;
    xs = another.xs;
    ys = another.ys;
    abbr = another.abbr;
    return *this;
}

UCurve &UCurve::operator=(UCurve &&another) {
    descriptor = std::move(another.descriptor);
    xs = another.xs;
    ys = another.ys;
    abbr = another.abbr;
    return *this;
}

bool UCurve::isEmpty() const {
    return xs.isEmpty() || allZero(ys);
}

bool UCurve::isEmptyBetween(int x0, int x1, int defaultValue) const {
    if (sample(x0) != defaultValue || sample(x1) != defaultValue) {
        return false;
    }
    int idx = binarySearch(xs, x0);
    if (idx < 0) {
        idx = ~idx;
    }
    while (idx < xs.size() && xs[idx] <= x1) {
        if (ys[idx] != defaultValue) {
            return false;
        }
        idx++;
    }
    return true;
}

int UCurve::sample(int x) const {
    int idx = binarySearch(xs, x);
    if (idx >= 0) {
        return ys[idx];
    }
    idx = ~idx;
    if (idx > 0 && idx < xs.size()) {
        return qRound(MusicMath::Linear(xs[idx - 1], xs[idx], ys[idx - 1], ys[idx], x));
    }
    return 0;
}

void UCurve::set(int x, int y, int lastX, int lastY) {
    x = qRound((float) x / interval) * interval;
    lastX = qRound((float) lastX / interval) * interval;
    if (x == lastX) {
        int leftY = sample(x - interval);
        int rightY = sample(x + interval);
        insert(x - interval, leftY);
        insert(x, y);
        insert(x + interval, rightY);
    } else if (x < lastX) {
        int leftY = sample(x - interval);
        deleteBetweenExclusive(x, lastX);
        insert(x - interval, leftY);
        insert(x, y);
    } else {
        int rightY = sample(x + interval);
        deleteBetweenExclusive(lastX, x);
        insert(x, y);
        insert(x + interval, rightY);
    }

    Q_UNUSED(lastY)
}

void UCurve::simplify() {
    if (xs.size() < 3) {
        return;
    }
    int first = 0;
    int last = xs.size() - 1;

    QList<int> toKeep{first, last};
    double tolerance = qMin(5.0, (descriptor->max - descriptor->min) * 0.005);
    simplify(first, last, tolerance, toKeep);
    std::sort(toKeep.begin(), toKeep.end());

    QList<int> newXs, newYs;
    for (int index : qAsConst(toKeep)) {
        newXs.append(xs[index]);
        newYs.append(ys[index]);
    }
    xs = newXs;
    ys = newYs;
}

void UCurve::simplify(int first, int last, double tolerance, QList<int> &toKeep) {
    double maxHeight = 0;
    int maxHeightIdx = 0;
    for (int index = first; index < last; index++) {
        double height =
            perpendicularDistance(xs[first], ys[first], xs[last], ys[last], xs[index], ys[index]);
        if (height > maxHeight) {
            maxHeight = height;
            maxHeightIdx = index;
        }
    }
    if (maxHeight > tolerance && maxHeightIdx != 0) {
        toKeep.append(maxHeightIdx);
        simplify(first, maxHeightIdx, tolerance, toKeep);
        simplify(maxHeightIdx, last, tolerance, toKeep);
    }
}
void UCurve::insert(int x, int y) {
    int idx = std::distance(xs.begin(), std::lower_bound(xs.begin(), xs.end(), x));
    if (idx >= 0) {
        ys[idx] = y;
        return;
    }
    idx = ~idx;
    xs.insert(idx, x);
    ys.insert(idx, y);
}

void UCurve::deleteBetweenExclusive(int x1, int x2) {
    int li = binarySearch(xs, x1);
    if (li >= 0) {
        li++;
    } else {
        li = ~li;
    }
    int ri = binarySearch(xs, x2);
    if (ri >= 0) {
        ri--;
    } else {
        ri = ~ri - 1;
    }
    if (ri >= li) {
        removeRange(xs, li, ri - li + 1);
        removeRange(ys, li, ri - li + 1);
    }
}

double UCurve::perpendicularDistance(int x, int y, int x1, int y1, int x2, int y2) {
    double area = 0.5 * qAbs(x1 * (y2 - y) + x2 * (y - y1) + x * (y1 - y2));
    double bottom = qSqrt(qPow(x1 - x2, 2) + qPow(y1 - y2, 2));
    return area / bottom * 2;
}
