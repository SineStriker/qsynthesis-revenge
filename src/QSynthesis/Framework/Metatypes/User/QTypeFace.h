#ifndef QTYPEFACE_H
#define QTYPEFACE_H

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QStringList>

class QTypeFace {
public:
    QTypeFace();
    QTypeFace(const QColor &color, double width);
    ~QTypeFace();

private:
    void init();

public:
    QStringList toStringList() const;

    QFont font() const;
    void setFont(const QFont &font);

    double widthF() const;
    void setWidthF(double width);

    QColor color() const;
    void setColor(const QColor &color);

private:
    QFont m_font;
    QColor m_color;

public:
    static QTypeFace fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QTypeFace &tf);
};

Q_DECLARE_METATYPE(QTypeFace)

#endif // QTYPEFACE_H
