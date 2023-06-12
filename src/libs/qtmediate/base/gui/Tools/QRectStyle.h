#ifndef QRECTSTYLE_H
#define QRECTSTYLE_H

#include <QColor>
#include <QMetaType>
#include <QRect>
#include <QString>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QRectStyle : public QRect {
public:
    QRectStyle();

private:
    void init();

public:
    QStringList toStringList() const;

    QRectF rect() const;
    void setRect(const QRectF &rect);

    QMarginsF margins() const;

    double radius() const;
    void setRadius(double radius);

    QList<QColor> colors() const;
    void setColors(const QList<QColor> &colors);

    inline void setColor(const QColor &color) {
        setColors({color});
    }

    QColor color() const;
    QColor color2() const;
    QColor color3() const;
    QColor color4() const;

protected:
    QRectF m_rect;
    double m_radius;

    QList<QColor> m_colors;

public:
    static QRectStyle fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QRectStyle &info);
};

Q_DECLARE_METATYPE(QRectStyle)

#endif // QRECTSTYLE_H
