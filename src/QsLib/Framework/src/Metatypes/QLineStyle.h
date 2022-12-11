#ifndef QLINESTYLE_H
#define QLINESTYLE_H

#include <QPen>

#include "qsframework_global.h"

class QSFRAMEWORK_API QLineStyle : public QPen {
public:
    QLineStyle();
    QLineStyle(Qt::PenStyle style);
    QLineStyle(const QColor &color);
    QLineStyle(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
               Qt::PenCapStyle c = Qt::SquareCap, Qt::PenJoinStyle j = Qt::BevelJoin);
    QLineStyle(const QPen &pen) noexcept;
    ~QLineStyle();

public:
    QStringList toStringList() const;
    static QLineStyle fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QLineStyle &pen);
};

Q_DECLARE_METATYPE(QLineStyle)

#endif // QLINESTYLE_H
