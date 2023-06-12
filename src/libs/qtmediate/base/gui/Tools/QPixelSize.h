#ifndef QPIXELSIZE_H
#define QPIXELSIZE_H

#include <QDebug>
#include <QString>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QPixelSize {
public:
    QPixelSize();
    QPixelSize(int pixel);
    QPixelSize(double pixel);
    ~QPixelSize();

public:
    QString toString() const;

    int value() const;
    void setValue(int value);

    double valueF() const;
    void setValueF(double value);

    operator int() const;
    operator double() const;

protected:
    double m_value;

public:
    static QPixelSize fromString(const QString &string);

    friend QDebug operator<<(QDebug debug, const QPixelSize &pixel);
};

Q_DECLARE_METATYPE(QPixelSize)

#endif // QPIXELSIZE_H
