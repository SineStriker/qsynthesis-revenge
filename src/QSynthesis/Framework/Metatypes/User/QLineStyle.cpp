#include "QLineStyle.h"
#include "QMetaTypeUtils.h"

#include "MathHelper.h"

#include <QDebug>

QLineStyle::QLineStyle() {
}

QLineStyle::QLineStyle(Qt::PenStyle style) : QPen(style) {
}

QLineStyle::QLineStyle(const QColor &color) : QPen(color) {
}

QLineStyle::QLineStyle(const QBrush &brush, qreal width, Qt::PenStyle s, Qt::PenCapStyle c,
                       Qt::PenJoinStyle j)
    : QPen(brush, width, s, c, j) {
}

QLineStyle::QLineStyle(const QPen &pen) noexcept : QPen(pen) {
}

QLineStyle::~QLineStyle() {
}

QStringList QLineStyle::toStringList() const {
    return {MetaFunctionName(),
            QString("%1%2,%3,%4,%5,%6")
                .arg(QString::number(widthF()), QLatin1String(PixelSizeUnit),
                     QLatin1String(color().name().toLatin1()), LineStyleToString(style()),
                     CapStyleToString(capStyle()), JoinStyleToString(joinStyle()))};
}

QLineStyle QLineStyle::fromStringList(const QStringList &stringList) {
    QLineStyle res;
    if (stringList.size() == 2 &&
        (!stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)
         /*||!stringList.front().compare(QLatin1String(QCssCustomValue_Pen_2),
Qt::CaseInsensitive)*/
         )) {
        QStringList valueList = SplitStringByComma(stringList.back());
        for (int i = 0; i < valueList.size(); ++i) {
            const QString &val = valueList.at(i).simplified();
            switch (i) {
            case 0: {
                res.setBrush(QBrush(Math::CssStringToColor(val)));
                break;
            };
            case 1: {
                QString str = val;
                QLatin1String px(PixelSizeUnit);
                if (str.endsWith(px, Qt::CaseInsensitive)) {
                    str.chop(px.size());
                }
                bool isNum;
                double num = str.toDouble(&isNum);
                if (isNum) {
                    res.setWidthF(num);
                }
                break;
            }
            case 2: {
                bool ok;
                Qt::PenStyle style = StringToLineStyle(val, &ok);
                if (ok) {
                    res.setStyle(style);
                }
                break;
            }
            case 3: {
                bool ok;
                Qt::PenCapStyle style = StringToCapStyle(val, &ok);
                if (ok) {
                    res.setCapStyle(style);
                }
                break;
            }
            case 4: {
                bool ok;
                Qt::PenJoinStyle style = StringToJoinStyle(val, &ok);
                if (ok) {
                    res.setJoinStyle(style);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    return res;
}

QLatin1String QLineStyle::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_Pen);
}

QDebug operator<<(QDebug debug, const QLineStyle &pen) {
    debug.nospace() << QString("QLineStyle(%1%2,%3,%4,%5,%6)")
                           .arg(QString::number(pen.widthF()), QLatin1String(PixelSizeUnit),
                                QLatin1String(pen.color().name().toLatin1()),
                                LineStyleToString(pen.style()), CapStyleToString(pen.capStyle()),
                                JoinStyleToString(pen.joinStyle()));
    return debug;
}
