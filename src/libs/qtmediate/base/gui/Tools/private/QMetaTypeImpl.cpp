#include "QMetaTypeImpl.h"
#include "QMarginsImpl.h"
#include "private/QMetaTypeUtils.h"

#include <QFileInfo>
#include <QMap>
#include <QMargins>

namespace QMetaTypeImpl {

    QStringList MarginsToStringList(const QMargins &margins) {
        return QMarginsImpl::toStringList(margins);
    }

    QMargins StringListToMargins(const QStringList &stringList) {
        return QMarginsImpl::fromStringList(stringList);
    }

    QString MarginsToString(const QMargins &margins) {
        return QString("%1%2").arg(QString::number(margins.left()), QLatin1String(PixelSizeUnit));
    }

    QMargins StringToMargins(const QString &string) {
        QPixelSize px = QPixelSize::fromString(string);
        int value = px.value();
        return {value, value, value, value};
    }

    QStringList PenToStringList(const QLineStyle &pen) {
        return pen.toStringList();
    }

    QLineStyle StringListToPen(const QStringList &stringList) {
        return QLineStyle::fromStringList(stringList);
    }

    QStringList DataUriToStringList(const QDataUri &uri) {
        return uri.toStringList();
    }

    QDataUri StringListToDataUri(const QStringList &stringList) {
        return QDataUri::fromStringList(stringList);
    }

    QStringList SvgUriToStringList(const QSvgUri &uri) {
        return uri.toStringList();
    }

    QSvgUri StringListToSvgUri(const QStringList &stringList) {
        return QSvgUri::fromStringList(stringList);
    }

    QString SvgUriToString(const QSvgUri &uri) {
        return uri.filename();
    }

    QSvgUri StringToSvgUri(const QString &string) {
        QFileInfo info(string);
        if (info.isFile() && string.compare(QLatin1String(NoneValue), Qt::CaseInsensitive)) {
            QSvgUri uri;
            uri.setFilename(info.fileName());
            return uri;
        }
        return QSvgUri();
    }

    QStringList TypeFaceToStringList(const QTypeFace &tf) {
        return tf.toStringList();
    }

    QTypeFace StringListToTypeFace(const QStringList &stringList) {
        return QTypeFace::fromStringList(stringList);
    }

    QString PixelSizeToString(const QPixelSize &pixel) {
        return pixel.toString();
    }

    QPixelSize StringToPixelSize(const QString &string) {
        return QPixelSize::fromString(string);
    }

    double PixelSizeToDouble(const QPixelSize &pixel) {
        return pixel.value();
    }

    QPixelSize DoubleToPixelSize(double size) {
        return {size};
    }

    QColorList StringListColorList(const QStringList &stringList) {
        return QColorList::fromStringList(stringList);
    }

    QStringList ColorListToStringList(const QColorList &colors) {
        return colors.toStringList();
    }

    QRectStyle StringListToRectStyle(const QStringList &stringList) {
        return QRectStyle::fromStringList(stringList);
    }

    QStringList RectStyleToStringList(const QRectStyle &rectStyle) {
        return rectStyle.toStringList();
    }

    QTypeList StringListToTypeList(const QStringList &stringList) {
        return QTypeList::fromStringList(stringList);
    }

    QStringList TypeListToStringList(const QTypeList &types) {
        return types.toStringList();
    }

    QTypeMap StringListToTypeMap(const QStringList &stringList) {
        return QTypeMap::fromStringList(stringList);
    }
    QStringList TypeMapToStringList(const QTypeMap &types) {
        return types.toStringList();
    }

}