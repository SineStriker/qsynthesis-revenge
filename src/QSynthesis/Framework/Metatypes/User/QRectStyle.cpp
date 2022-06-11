#include "QRectStyle.h"
#include "QMetaTypeUtils.h"

#include <QDebug>

QRectStyle::QRectStyle() {
    init();
}

void QRectStyle::init() {
    m_radius = 0;
}

QStringList QRectStyle::toStringList() const {
    return {};
}

QPen QRectStyle::pen() const {
    return m_pen;
}

void QRectStyle::setPen(const QPen &pen) {
    m_pen = pen;
}

QBrush QRectStyle::brush() const {
    return m_brush;
}

void QRectStyle::setBrush(const QBrush &brush) {
    m_brush = brush;
}

double QRectStyle::radius() const {
    return m_radius;
}

void QRectStyle::setRadius(double radius) {
    m_radius = radius;
}

QRectStyle QRectStyle::fromStringList(const QStringList &stringList) {
    return QRectStyle();
}

QLatin1String QRectStyle::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_RectStyle);
}

QDebug operator<<(QDebug debug, const QRectStyle &info) {
    debug.noquote().nospace() << "QRectInfo(" << info.m_pen << ", " << info.m_brush << ", "
                              << info.m_radius << QLatin1String(PixelSizeUnit) << ")";
    return debug;
}
