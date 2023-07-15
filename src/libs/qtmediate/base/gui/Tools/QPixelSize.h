#ifndef QPIXELSIZE_H
#define QPIXELSIZE_H

#include <QMetaType>
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

public:
    static QPixelSize fromString(const QString &string);

    friend QDebug operator<<(QDebug debug, const QPixelSize &pixel);

private:
    double m_value;
};

Q_DECLARE_TYPEINFO(QPixelSize, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QPixelSize)

#endif // QPIXELSIZE_H
