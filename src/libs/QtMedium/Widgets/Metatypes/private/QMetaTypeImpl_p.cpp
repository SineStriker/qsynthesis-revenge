#include "QMetaTypeImpl_p.h"

#include <QMargins>
#include <QMetaType>

#include "QMetaTypeImpl.h"

using namespace QMetaTypeImpl;

void Register_QMetaTypeImpl() {
    QMetaType::registerConverter<QStringList, QMargins>(StringListToMargins);
    QMetaType::registerConverter<QMargins, QStringList>(MarginsToStringList);
    QMetaType::registerConverter<QString, QMargins>(StringToMargins);
    QMetaType::registerConverter<QMargins, QString>(MarginsToString);

    QMetaType::registerConverter<QStringList, QLineStyle>(StringListToPen);
    QMetaType::registerConverter<QLineStyle, QStringList>(PenToStringList);

    QMetaType::registerConverter<QStringList, QDataUri>(StringListToDataUri);
    QMetaType::registerConverter<QDataUri, QStringList>(DataUriToStringList);

    QMetaType::registerConverter<QStringList, QSvgUri>(StringListToSvgUri);
    QMetaType::registerConverter<QSvgUri, QStringList>(SvgUriToStringList);

    QMetaType::registerConverter<QStringList, QTypeFace>(StringListToTypeFace);
    QMetaType::registerConverter<QTypeFace, QStringList>(TypeFaceToStringList);

    QMetaType::registerConverter<QString, QPixelSize>(StringToPixelSize);
    QMetaType::registerConverter<QPixelSize, QString>(PixelSizeToString);
    QMetaType::registerConverter<double, QPixelSize>(DoubleToPixelSize);
    QMetaType::registerConverter<QPixelSize, double>(PixelSizeToDouble);

    QMetaType::registerConverter<QStringList, QColorList>(StringListColorList);
    QMetaType::registerConverter<QColorList, QStringList>(ColorListToStringList);

    QMetaType::registerConverter<QStringList, QRectStyle>(StringListToRectStyle);
    QMetaType::registerConverter<QRectStyle, QStringList>(RectStyleToStringList);

    QMetaType::registerConverter<QStringList, QTypeList>(StringListToTypeList);
    QMetaType::registerConverter<QTypeList, QStringList>(TypeListToStringList);
}
