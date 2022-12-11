#ifndef QMETATYPEIMPL_H
#define QMETATYPEIMPL_H

#include <QDebug>

#include "QColorList.h"
#include "QDataUri.h"
#include "QLineStyle.h"
#include "QMarginsImpl.h"
#include "QPixelSize.h"
#include "QRectStyle.h"
#include "QSvgUri.h"
#include "QTypeFace.h"
#include "QTypeList.h"

#include "qsframework_global.h"

namespace QMetaTypeImpl {

    QSFRAMEWORK_API QStringList MarginsToStringList(const QMargins &margins);

    QSFRAMEWORK_API QMargins StringListToMargins(const QStringList &stringList);

    QSFRAMEWORK_API QString MarginsToString(const QMargins &margins);

    QSFRAMEWORK_API QMargins StringToMargins(const QString &string);

    QSFRAMEWORK_API QStringList PenToStringList(const QLineStyle &pen);

    QSFRAMEWORK_API QLineStyle StringListToPen(const QStringList &stringList);

    QSFRAMEWORK_API QStringList DataUriToStringList(const QDataUri &uri);

    QSFRAMEWORK_API QDataUri StringListToDataUri(const QStringList &stringList);

    QSFRAMEWORK_API QStringList SvgUriToStringList(const QSvgUri &uri);

    QSFRAMEWORK_API QSvgUri StringListToSvgUri(const QStringList &stringList);

    QSFRAMEWORK_API QStringList TypeFaceToStringList(const QTypeFace &tf);

    QSFRAMEWORK_API QString SvgUriToString(const QSvgUri &uri);

    QSFRAMEWORK_API QSvgUri StringToSvgUri(const QString &string);

    QSFRAMEWORK_API QTypeFace StringListToTypeFace(const QStringList &stringList);

    QSFRAMEWORK_API QString PixelSizeToString(const QPixelSize &pixel);

    QSFRAMEWORK_API QPixelSize StringToPixelSize(const QString &string);

    QSFRAMEWORK_API double PixelSizeToDouble(const QPixelSize &pixel);

    QSFRAMEWORK_API QPixelSize DoubleToPixelSize(double size);

    QSFRAMEWORK_API QColorList StringListColorList(const QStringList &stringList);

    QSFRAMEWORK_API QStringList ColorListToStringList(const QColorList &colors);

    QSFRAMEWORK_API QRectStyle StringListToRectStyle(const QStringList &stringList);

    QSFRAMEWORK_API QStringList RectStyleToStringList(const QRectStyle &rectStyle);

    QSFRAMEWORK_API QTypeList StringListToTypeList(const QStringList &stringList);

    QSFRAMEWORK_API QStringList TypeListToStringList(const QTypeList &types);

}; // namespace QMetaTypeImpl

#endif // QMETATYPEIMPL_H
