#include "QRectStyle.h"
#include "private/QMetaTypeUtils.h"

#include "QMCss.h"

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

QRectF QRectStyle::rect() const {
    return m_rect;
}

void QRectStyle::setRect(const QRectF &rect) {
    m_rect = rect;
}

QMarginsF QRectStyle::margins() const {
    return {m_rect.left(), m_rect.top(), m_rect.width(), m_rect.height()};
}

double QRectStyle::radius() const {
    return m_radius;
}

void QRectStyle::setRadius(double radius) {
    m_radius = radius;
}

QRectStyle QRectStyle::fromStringList(const QStringList &stringList) {
    QRectStyle res;
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList valueList = SplitStringByComma(stringList.back());

        QVector<double> x;
        int i = 0;
        for (; i < valueList.size(); ++i) {
            QString str = valueList.at(i).simplified();
            QLatin1String px(PixelSizeUnit);
            if (str.endsWith(px, Qt::CaseInsensitive)) {
                str.chop(px.size());
            }
            bool isNum;
            double num = str.toDouble(&isNum);
            if (isNum) {
                x.push_back(num);
            } else {
                break;
            }
        }

        while (x.size() < 5) {
            x.prepend(0);
        }

        res.m_rect = QRectF(x.at(0), x.at(1), x.at(2), x.at(3));
        res.m_radius = x.at(4);

        if (i < valueList.size()) {
            const QString &strColor = valueList.at(i).simplified();
            if (strColor.startsWith('(') && strColor.endsWith(')')) {
                QStringList colorStrings = SplitStringByComma(strColor.mid(1, strColor.size() - 2));
                QList<QColor> colors;
                for (const auto &item : qAsConst(colorStrings)) {
                    colors.append(QMCss::CssStringToColor(item.simplified()));
                }
                res.m_colors = colors;
            } else {
                res.m_colors = {QMCss::CssStringToColor(strColor)};
            }
        }
    }
    return res;
}

QLatin1String QRectStyle::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_RectStyle);
}

QDebug operator<<(QDebug debug, const QRectStyle &info) {
    debug.noquote().nospace() << "QRectStyle(" << info.m_rect << ", " << info.m_radius << ", " << info.m_colors << ")";
    return debug;
}

QList<QColor> QRectStyle::colors() const {
    return m_colors;
}

void QRectStyle::setColors(const QList<QColor> &colors) {
    m_colors = colors;
}

QColor QRectStyle::color() const {
    return m_colors.isEmpty() ? Qt::transparent : m_colors.front();
}

QColor QRectStyle::color2() const {
    return m_colors.size() < 2 ? color() : m_colors.at(1);
}

QColor QRectStyle::color3() const {
    return m_colors.size() < 3 ? color2() : m_colors.at(2);
}

QColor QRectStyle::color4() const {
    return m_colors.size() < 4 ? color3() : m_colors.at(3);
}
