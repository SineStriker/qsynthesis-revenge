#ifndef QMETATYPEIMPL_H
#define QMETATYPEIMPL_H

#include <QDebug>

#include "QColorList.h"
#include "QDataUri.h"
#include "QLineStyle.h"
#include "QPixelSize.h"
#include "QRectStyle.h"
#include "QSvgUri.h"
#include "QTypeFace.h"
#include "QTypeList.h"
#include "QTypeMap.h"

namespace QMetaTypeImpl {

    QStringList MarginsToStringList(const QMargins &margins);

    QMargins StringListToMargins(const QStringList &stringList);

    QString MarginsToString(const QMargins &margins);

    QMargins StringToMargins(const QString &string);

    QStringList PenToStringList(const QLineStyle &pen);

    QLineStyle StringListToPen(const QStringList &stringList);

    QStringList DataUriToStringList(const QDataUri &uri);

    QDataUri StringListToDataUri(const QStringList &stringList);

    QStringList SvgUriToStringList(const QSvgUri &uri);

    QSvgUri StringListToSvgUri(const QStringList &stringList);

    QStringList TypeFaceToStringList(const QTypeFace &tf);

    QString SvgUriToString(const QSvgUri &uri);

    QSvgUri StringToSvgUri(const QString &string);

    QTypeFace StringListToTypeFace(const QStringList &stringList);

    QString PixelSizeToString(const QPixelSize &pixel);

    QPixelSize StringToPixelSize(const QString &string);

    double PixelSizeToDouble(const QPixelSize &pixel);

    QPixelSize DoubleToPixelSize(double size);

    QColorList StringListColorList(const QStringList &stringList);

    QStringList ColorListToStringList(const QColorList &colors);

    QRectStyle StringListToRectStyle(const QStringList &stringList);

    QStringList RectStyleToStringList(const QRectStyle &rectStyle);

    QTypeList StringListToTypeList(const QStringList &stringList);

    QStringList TypeListToStringList(const QTypeList &types);

    QTypeMap StringListToTypeMap(const QStringList &stringList);

    QStringList TypeMapToStringList(const QTypeMap &types);

};

#endif // QMETATYPEIMPL_H
