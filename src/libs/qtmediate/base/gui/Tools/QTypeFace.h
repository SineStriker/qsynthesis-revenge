#ifndef QTYPEFACE_H
#define QTYPEFACE_H

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QStringList>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QTypeFace {
public:
    QTypeFace();
    QTypeFace(const QColor &color, int pixelSize);
    QTypeFace(const QColor &color, double pointSize);
    QTypeFace(const QList<QColor> &colors, int pixelSize);
    QTypeFace(const QList<QColor> &colors, double pointSize);
    ~QTypeFace();

private:
    void init();

public:
    QStringList toStringList() const;

    bool isDefaultFont() const;

    QFont font() const;
    void setFont(const QFont &font);

    double pixelSize() const;
    void setPixelSize(double pixelSize);

    double pointSize() const;
    void setPointSize(double pointSize);

    double weight() const;
    void setWeight(double weight);

    QList<QColor> colors() const;
    void setColors(const QList<QColor> &colors);

    inline void setColor(const QColor &color) {
        setColors({color});
    }

    QColor color() const;
    QColor color2() const;
    QColor color3() const;
    QColor color4() const;

private:
    QFont m_font;

    bool m_defaultFont;
    double m_pixelSize;
    double m_pointSize;
    double m_weight;

    QList<QColor> m_colors;

public:
    static QTypeFace fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QTypeFace &tf);
};

Q_DECLARE_METATYPE(QTypeFace)

#endif // QTYPEFACE_H
