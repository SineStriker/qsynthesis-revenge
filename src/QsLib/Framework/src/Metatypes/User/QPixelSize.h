#ifndef QPIXELSIZE_H
#define QPIXELSIZE_H

#include <QDebug>
#include <QString>

#include "qsframework_global.h"

class QSFRAMEWORK_API QPixelSize {
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

protected:
    double m_value;

public:
    static QPixelSize fromString(const QString &string);

    friend QDebug operator<<(QDebug debug, const QPixelSize &pixel);
};

Q_DECLARE_METATYPE(QPixelSize)

#endif // QPIXELSIZE_H
