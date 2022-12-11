#include "QMetaTypeImpl.h"
#include "MathHelper.h"
#include "private/QMetaTypeUtils.h"

#include <QFileInfo>
#include <QMap>
#include <QMargins>

QStringList QMetaTypeImpl::MarginsToStringList(const QMargins &margins) {
    return QMarginsImpl::toStringList(margins);
}

QMargins QMetaTypeImpl::StringListToMargins(const QStringList &stringList) {
    return QMarginsImpl::fromStringList(stringList);
}

QString QMetaTypeImpl::MarginsToString(const QMargins &margins) {
    return QString("%1%2").arg(QString::number(margins.left()), QLatin1String(PixelSizeUnit));
}

QMargins QMetaTypeImpl::StringToMargins(const QString &string) {
    QPixelSize px = QPixelSize::fromString(string);
    int value = px.value();
    return QMargins(value, value, value, value);
}

QStringList QMetaTypeImpl::PenToStringList(const QLineStyle &pen) {
    return pen.toStringList();
}

QLineStyle QMetaTypeImpl::StringListToPen(const QStringList &stringList) {
    return QLineStyle::fromStringList(stringList);
}

QStringList QMetaTypeImpl::DataUriToStringList(const QDataUri &uri) {
    return uri.toStringList();
}

QDataUri QMetaTypeImpl::StringListToDataUri(const QStringList &stringList) {
    return QDataUri::fromStringList(stringList);
}

QStringList QMetaTypeImpl::SvgUriToStringList(const QSvgUri &uri) {
    return uri.toStringList();
}

QSvgUri QMetaTypeImpl::StringListToSvgUri(const QStringList &stringList) {
    return QSvgUri::fromStringList(stringList);
}

QString QMetaTypeImpl::SvgUriToString(const QSvgUri &uri) {
    return uri.filename();
}

QSvgUri QMetaTypeImpl::StringToSvgUri(const QString &string) {
    QFileInfo info(string);
    if (info.isFile() && string.compare(QLatin1String(NoneValue), Qt::CaseInsensitive)) {
        QSvgUri uri;
        uri.setFilename(info.fileName());
        return uri;
    }
    return QSvgUri();
}

QStringList QMetaTypeImpl::TypeFaceToStringList(const QTypeFace &tf) {
    return tf.toStringList();
}

QTypeFace QMetaTypeImpl::StringListToTypeFace(const QStringList &stringList) {
    return QTypeFace::fromStringList(stringList);
}

QString QMetaTypeImpl::PixelSizeToString(const QPixelSize &pixel) {
    return pixel.toString();
}

QPixelSize QMetaTypeImpl::StringToPixelSize(const QString &string) {
    return QPixelSize::fromString(string);
}

double QMetaTypeImpl::PixelSizeToDouble(const QPixelSize &pixel) {
    return pixel.value();
}

QPixelSize QMetaTypeImpl::DoubleToPixelSize(double size) {
    return QPixelSize(size);
}

QColorList QMetaTypeImpl::StringListColorList(const QStringList &stringList) {
    return QColorList::fromStringList(stringList);
}

QStringList QMetaTypeImpl::ColorListToStringList(const QColorList &colors) {
    return colors.toStringList();
}

QRectStyle QMetaTypeImpl::StringListToRectStyle(const QStringList &stringList) {
    return QRectStyle::fromStringList(stringList);
}

QStringList QMetaTypeImpl::RectStyleToStringList(const QRectStyle &rectStyle) {
    return rectStyle.toStringList();
}

QTypeList QMetaTypeImpl::StringListToTypeList(const QStringList &stringList) {
    return QTypeList::fromStringList(stringList);
}

QStringList QMetaTypeImpl::TypeListToStringList(const QTypeList &types) {
    return types.toStringList();
}
