#ifndef QRECTSTYLE_H
#define QRECTSTYLE_H

#include <QPen>
#include <QRect>
#include <QString>

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API QRectStyle : public QRect {
public:
    QRectStyle();

private:
    void init();

public:
    QStringList toStringList() const;

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    double radius() const;
    void setRadius(double radius);

protected:
    QPen m_pen;
    QBrush m_brush;
    double m_radius;

public:
    static QRectStyle fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QRectStyle &info);
};

Q_DECLARE_METATYPE(QRectStyle)

#endif // QRECTSTYLE_H
