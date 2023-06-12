#include "QMetaTypeUtils.h"
#include "QMarginsImpl.h"
#include "QMetaTypeImpl.h"

#include <QMetaType>

Qt::PenStyle StringToLineStyle(const QString &str, bool *ok) {
    Qt::PenStyle style = Qt::NoPen;
    bool success = true;
    if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_None), Qt::CaseInsensitive)) {
        style = Qt::NoPen;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_Solid), Qt::CaseInsensitive)) {
        style = Qt::SolidLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_Dash), Qt::CaseInsensitive)) {
        style = Qt::DashLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_Dot), Qt::CaseInsensitive)) {
        style = Qt::DotLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_DashDot), Qt::CaseInsensitive)) {
        style = Qt::DashDotLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_DashDotDot), Qt::CaseInsensitive)) {
        style = Qt::DashDotDotLine;
    } else {
        success = false;
    }
    if (ok) {
        *ok = success;
    }
    return style;
}

QLatin1String LineStyleToString(Qt::PenStyle style) {
    QLatin1String res;
    switch (style) {
        case Qt::NoPen:
            res = QLatin1String(QCssCustomValue_Pen_Line_None);
            break;
        case Qt::SolidLine:
            res = QLatin1String(QCssCustomValue_Pen_Line_Solid);
            break;
        case Qt::DashLine:
            res = QLatin1String(QCssCustomValue_Pen_Line_Dash);
            break;
        case Qt::DotLine:
            res = QLatin1String(QCssCustomValue_Pen_Line_Dot);
            break;
        case Qt::DashDotLine:
            res = QLatin1String(QCssCustomValue_Pen_Line_DashDot);
            break;
        case Qt::DashDotDotLine:
            res = QLatin1String(QCssCustomValue_Pen_Line_DashDotDot);
            break;
        default:
            res = QLatin1String(QCssCustomValue_Pen_Line_None);
            break;
    }
    return res;
}

Qt::PenCapStyle StringToCapStyle(const QString &str, bool *ok) {
    Qt::PenCapStyle style = Qt::FlatCap;
    bool success = true;
    if (!str.compare(QLatin1String(QCssCustomValue_Pen_Cap_Flat), Qt::CaseInsensitive)) {
        style = Qt::FlatCap;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Cap_Square), Qt::CaseInsensitive)) {
        style = Qt::SquareCap;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Cap_Round), Qt::CaseInsensitive)) {
        style = Qt::RoundCap;
    } else {
        success = false;
    }
    if (ok) {
        *ok = success;
    }
    return style;
}

QLatin1String CapStyleToString(Qt::PenCapStyle style) {
    QLatin1String res;
    switch (style) {
        case Qt::SquareCap:
            res = QLatin1String(QCssCustomValue_Pen_Cap_Square);
            break;
        case Qt::RoundCap:
            res = QLatin1String(QCssCustomValue_Pen_Cap_Round);
            break;
        default:
            res = QLatin1String(QCssCustomValue_Pen_Cap_Flat);
            break;
    }
    return res;
}

Qt::PenJoinStyle StringToJoinStyle(const QString &str, bool *ok) {
    Qt::PenJoinStyle style = Qt::BevelJoin;
    bool success = true;
    if (!str.compare(QLatin1String(QCssCustomValue_Pen_Join_Miter), Qt::CaseInsensitive)) {
        style = Qt::MiterJoin;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Join_Bevel), Qt::CaseInsensitive)) {
        style = Qt::BevelJoin;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Join_Round), Qt::CaseInsensitive)) {
        style = Qt::RoundJoin;
    } else {
        success = false;
    }
    if (ok) {
        *ok = success;
    }
    return style;
}

QLatin1String JoinStyleToString(Qt::PenJoinStyle style) {
    QLatin1String res;
    switch (style) {
        case Qt::MiterJoin:
            res = QLatin1String(QCssCustomValue_Pen_Join_Miter);
            break;
        case Qt::RoundJoin:
            res = QLatin1String(QCssCustomValue_Pen_Join_Round);
            break;
        default:
            res = QLatin1String(QCssCustomValue_Pen_Join_Bevel);
            break;
    }
    return res;
}

QStringList SplitStringByComma(const QString &str) {
    QStringList res;
    int level = 0;
    QString word;
    bool isCommented = false;
    bool isQuoted = false;
    bool isSingleQuoted = false;
    for (int i = 0; i < str.size(); ++i) {
        QChar ch = str.at(i);
        if (!isCommented) {
            if (!isSingleQuoted && ch == '\"') {
                isQuoted = !isQuoted;
                word.append(ch);
                continue;
            } else if (!isQuoted && ch == '\'') {
                isSingleQuoted = !isSingleQuoted;
                word.append(ch);
                continue;
            }
        }
        if (!isQuoted && !isSingleQuoted && i < str.size() - 1) {
            QChar nxt = str.at(i + 1);
            if (ch == '/' && nxt == '*') {
                isCommented = true;
                i++;
                continue;
            } else if (ch == '*' && nxt == '/') {
                isCommented = false;
                i++;
                continue;
            }
        }
        if (!isCommented) {
            if (level == 0 && ch == ',') {
                res.append(word);
                word.clear();
            } else {
                if (ch == '(') {
                    level++;
                } else if (ch == ')') {
                    level--;
                }
                word.append(ch);
            }
        }
    }
    if (!word.isEmpty()) {
        res.append(word);
    }
    return res;
}

QLatin1String TypeToFunctionName(int id) {
    QLatin1String res;
    if (id == qMetaTypeId<QMargins>()) {
        res = QMarginsImpl::MetaFunctionName();
    } else if (id == qMetaTypeId<QColorList>()) {
        res = QColorList::MetaFunctionName();
    } else if (id == qMetaTypeId<QDataUri>()) {
        res = QDataUri::MetaFunctionName();
    } else if (id == qMetaTypeId<QLineStyle>()) {
        res = QLineStyle::MetaFunctionName();
    } else if (id == qMetaTypeId<QRectStyle>()) {
        res = QRectStyle::MetaFunctionName();
    } else if (id == qMetaTypeId<QSvgUri>()) {
        res = QSvgUri::MetaFunctionName();
    } else if (id == qMetaTypeId<QTypeFace>()) {
        res = QTypeFace::MetaFunctionName();
    } else if (id == qMetaTypeId<QTypeList>()) {
        res = QTypeList::MetaFunctionName();
    }
    return res;
}

int FunctionNameToType(const QString &name) {
    int id = -1;
    if (!name.compare(QMarginsImpl::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QMargins>();
    } else if (!name.compare(QColorList::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QColorList>();
    } else if (!name.compare(QDataUri::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QDataUri>();
    } else if (!name.compare(QLineStyle::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QLineStyle>();
    } else if (!name.compare(QRectStyle::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QRectStyle>();
    } else if (!name.compare(QSvgUri::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QSvgUri>();
    } else if (!name.compare(QTypeFace::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QTypeFace>();
    } else if (!name.compare(QTypeList::MetaFunctionName(), Qt::CaseInsensitive)) {
        id = qMetaTypeId<QTypeList>();
    }
    return id;
}
