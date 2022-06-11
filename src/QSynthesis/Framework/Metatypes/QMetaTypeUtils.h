#ifndef QMETATYPEUTILS_H
#define QMETATYPEUTILS_H

#include <QVariant>
#include <qnamespace.h>

const char PixelSizeUnit[] = "px";

const char NoneValue[] = "none";

const char QCssCustomValue_Url[] = "url";
const char QCssCustomValue_Url_Data[] = "data";
const char QCssCustomValue_Url_Charset_Prefix[] = "charset=";

const char QCssCustomValue_Svg_Url[] = "svg";
const char QCssCustomValue_Svg_CurrentColor_Prefix[] = "currentColor=";

const char QCssCustomValue_TypeFace[] = "qtypeface";

const char QCssCustomValue_Margins[] = "qmargins";

const char QCssCustomValue_Pen[] = "qpen";
const char QCssCustomValue_Pen_2[] = "qlinestyle";
const char QCssCustomValue_Pen_Line_None[] = "none";
const char QCssCustomValue_Pen_Line_Solid[] = "solid";
const char QCssCustomValue_Pen_Line_Dash[] = "dash";
const char QCssCustomValue_Pen_Line_Dot[] = "dot";
const char QCssCustomValue_Pen_Line_DashDot[] = "dashdot";
const char QCssCustomValue_Pen_Line_DashDotDot[] = "dashdotdot";
const char QCssCustomValue_Pen_Cap_Flat[] = "flat";
const char QCssCustomValue_Pen_Cap_Square[] = "square";
const char QCssCustomValue_Pen_Cap_Round[] = "round";
const char QCssCustomValue_Pen_Join_Miter[] = "miter";
const char QCssCustomValue_Pen_Join_Bevel[] = "bevel";
const char QCssCustomValue_Pen_Join_Round[] = "round";

const char QCssCustomValue_ColorList[] = "qcolors";
const char QCssCustomValue_TypeList[] = "qvariants";
const char QCssCustomValue_List[] = "qlist";

const char QCssCustomValue_RectStyle[] = "qrectstyle";

Qt::PenStyle StringToLineStyle(const QString &str, bool *ok = nullptr);

QLatin1String LineStyleToString(Qt::PenStyle style);

Qt::PenCapStyle StringToCapStyle(const QString &str, bool *ok = nullptr);

QLatin1String CapStyleToString(Qt::PenCapStyle style);

Qt::PenJoinStyle StringToJoinStyle(const QString &str, bool *ok = nullptr);

QLatin1String JoinStyleToString(Qt::PenJoinStyle style);

QStringList SplitStringByComma(const QString &str);

QLatin1String TypeToFunctionName(int id);

int FunctionNameToType(const QString &name);

#endif // QMETATYPEUTILS_H
