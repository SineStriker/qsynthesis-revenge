#include "QPixelSize.h"
#include "QMetaTypeUtils.h"

QPixelSize::QPixelSize() : m_value(0) {
}

QPixelSize::QPixelSize(int pixel) : m_value(pixel) {
}

QPixelSize::QPixelSize(double pixel) : m_value(pixel) {
}

QPixelSize::~QPixelSize() {
}

QString QPixelSize::toString() const {
    return QString("%1%2").arg(QString::number(m_value), QLatin1String(PixelSizeUnit));
}

int QPixelSize::value() const {
    return m_value;
}

void QPixelSize::setValue(int value) {
    m_value = value;
}

double QPixelSize::valueF() const {
    return m_value;
}

void QPixelSize::setValueF(double value) {
    m_value = value;
}

QPixelSize QPixelSize::fromString(const QString &string) {
    QString str;
    QLatin1String px(PixelSizeUnit);
    if (string.endsWith(px, Qt::CaseInsensitive)) {
        str = string.chopped(px.size());
    } else {
        str = string;
    }

    QPixelSize p;
    bool isNum;
    int num = str.toInt(&isNum);
    if (isNum) {
        p.m_value = num;
    }
    return p;
}

QDebug operator<<(QDebug debug, const QPixelSize &pixel) {
    debug.nospace() << pixel.toString();
    return debug;
}
