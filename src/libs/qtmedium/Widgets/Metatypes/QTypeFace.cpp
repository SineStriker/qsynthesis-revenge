#include "QTypeFace.h"

#include "QMCss.h"
#include "private/QMetaTypeUtils.h"

#include <QApplication>

QTypeFace::QTypeFace() {
    init();
}

QTypeFace::QTypeFace(const QColor &color, double width) {
    init();
    setColor(color);
    setWidthF(width);
}

QTypeFace::~QTypeFace() {
}

void QTypeFace::init() {
    m_defaultFont = true;
    m_font = QApplication::font();
}

QStringList QTypeFace::toStringList() const {
    return {MetaFunctionName(), QString("%1,%2%3,%4")
                                    .arg(m_color.name(),
                                         (m_font.pixelSize() > 0) ? QString::number(m_font.pixelSize())
                                                                  : QString::number(m_font.pointSizeF()),
                                         QLatin1String(PixelSizeUnit), m_font.toString())};
}

QFont QTypeFace::font() const {
    return m_font;
}

void QTypeFace::setFont(const QFont &font) {
    m_font = font;
    m_defaultFont = false;
}

bool QTypeFace::isDefaultFont() const {
    return m_defaultFont;
}

double QTypeFace::widthF() const {
    return m_font.pointSizeF();
}

void QTypeFace::setWidthF(double width) {
    m_font.setPointSizeF(width);
}

QColor QTypeFace::color() const {
    return m_color;
}

void QTypeFace::setColor(const QColor &color) {
    m_color = color;
}

QTypeFace QTypeFace::fromStringList(const QStringList &stringList) {
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList content = SplitStringByComma(stringList.back());
        for (auto it = content.begin(); it != content.end(); ++it) {
            *it = it->simplified();
        }
        if (!content.isEmpty()) {
            QTypeFace tf;

            QString strColor = content.front();
            int pixelSize = -1;
            double pointSize = -1;
            int weight = -1;
            tf.setColor(QMCss::CssStringToColor(strColor));

            if (content.size() > 1) {
                QString strPixel = content.at(1);
                QLatin1String px(PixelSizeUnit);
                if (strPixel.endsWith(px, Qt::CaseInsensitive)) {
                    strPixel.chop(px.size());
                    bool isNum;
                    int num = strPixel.toInt(&isNum);
                    if (isNum) {
                        pixelSize = num;
                    }
                } else {
                    bool isNum;
                    double num = strPixel.toDouble(&isNum);
                    if (isNum) {
                        pointSize = num;
                    }
                }
            }
            if (content.size() > 2) {
                QString strWeight = content.at(2);
                bool isNum;
                int num = strWeight.toDouble(&isNum);
                if (isNum) {
                    weight = num;
                }
            }
            if (pixelSize > 0) {
                tf.m_font.setPixelSize(pixelSize);
            } else if (pointSize > 0) {
                tf.m_font.setPointSizeF(pointSize);
            }
            if (weight > 0) {
                tf.m_font.setWeight(weight);
            }
            return tf;
        }
    }
    return QTypeFace();
}

QLatin1String QTypeFace::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_TypeFace);
}

QDebug operator<<(QDebug debug, const QTypeFace &tf) {
    debug.noquote().nospace() << "QTypeFace(" << tf.m_color << ", " << tf.m_font << ")";
    return debug;
}
