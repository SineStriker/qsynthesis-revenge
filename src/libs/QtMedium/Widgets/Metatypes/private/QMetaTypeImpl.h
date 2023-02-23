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

#include "QMWidgetsGlobal.h"

namespace QMetaTypeImpl {

    QMWIDGETS_API QStringList MarginsToStringList(const QMargins &margins);

    QMWIDGETS_API QMargins StringListToMargins(const QStringList &stringList);

    QMWIDGETS_API QString MarginsToString(const QMargins &margins);

    QMWIDGETS_API QMargins StringToMargins(const QString &string);

    QMWIDGETS_API QStringList PenToStringList(const QLineStyle &pen);

    QMWIDGETS_API QLineStyle StringListToPen(const QStringList &stringList);

    QMWIDGETS_API QStringList DataUriToStringList(const QDataUri &uri);

    QMWIDGETS_API QDataUri StringListToDataUri(const QStringList &stringList);

    QMWIDGETS_API QStringList SvgUriToStringList(const QSvgUri &uri);

    QMWIDGETS_API QSvgUri StringListToSvgUri(const QStringList &stringList);

    QMWIDGETS_API QStringList TypeFaceToStringList(const QTypeFace &tf);

    QMWIDGETS_API QString SvgUriToString(const QSvgUri &uri);

    QMWIDGETS_API QSvgUri StringToSvgUri(const QString &string);

    QMWIDGETS_API QTypeFace StringListToTypeFace(const QStringList &stringList);

    QMWIDGETS_API QString PixelSizeToString(const QPixelSize &pixel);

    QMWIDGETS_API QPixelSize StringToPixelSize(const QString &string);

    QMWIDGETS_API double PixelSizeToDouble(const QPixelSize &pixel);

    QMWIDGETS_API QPixelSize DoubleToPixelSize(double size);

    QMWIDGETS_API QColorList StringListColorList(const QStringList &stringList);

    QMWIDGETS_API QStringList ColorListToStringList(const QColorList &colors);

    QMWIDGETS_API QRectStyle StringListToRectStyle(const QStringList &stringList);

    QMWIDGETS_API QStringList RectStyleToStringList(const QRectStyle &rectStyle);

    QMWIDGETS_API QTypeList StringListToTypeList(const QStringList &stringList);

    QMWIDGETS_API QStringList TypeListToStringList(const QTypeList &types);

};

#endif // QMETATYPEIMPL_H
