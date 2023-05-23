#include "QTypeFace.h"

#include "QMCss.h"
#include "private/QMetaTypeUtils.h"

#include <private/qfont_p.h>

#include <QApplication>

QTypeFace::QTypeFace() {
    init();
}

QTypeFace::QTypeFace(const QColor &color, int pixelSize) {
}

QTypeFace::QTypeFace(const QColor &color, double width) {
    init();
    setColor(color);
    setPointSize(width);
}

QTypeFace::~QTypeFace() {
}

void QTypeFace::init() {
    m_defaultFont = true;
    m_font = QApplication::font();

    m_pixelSize = m_font.pixelSize();
    m_pointSize = m_font.pointSizeF();
    m_weight = m_font.weight();
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
    m_defaultFont = false;
    m_font = font;

    m_pixelSize = m_font.pixelSize();
    m_pointSize = m_font.pointSizeF();
    m_weight = m_font.weight();
}

bool QTypeFace::isDefaultFont() const {
    return m_defaultFont;
}

double QTypeFace::pixelSize() const {
    return m_pixelSize;
}

void QTypeFace::setPixelSize(double pixelSize) {
    m_pixelSize = pixelSize;
    m_font.setPixelSize(pixelSize > 0 ? pixelSize : 1);
}

double QTypeFace::pointSize() const {
    return m_pointSize;
}

void QTypeFace::setPointSize(double pointSize) {
    m_pointSize = pointSize;
    m_font.setPointSizeF(pointSize > 0 ? pointSize : 1);
}

double QTypeFace::weight() const {
    return m_weight;
}

void QTypeFace::setWeight(double weight) {
    m_weight = weight;
    m_font.setWeight((weight > 0 && weight < 100) ? weight : 1);
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
        for (auto &item : content) {
            item = item.simplified();
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
                const QString &strWeight = content.at(2);
                bool isNum;
                int num = strWeight.toInt(&isNum);
                if (isNum) {
                    weight = num;
                }
            }
            if (pixelSize > 0) {
                tf.setPixelSize(pixelSize);
            } else if (pointSize > 0) {
                tf.setPointSize(pointSize);
            }
            if (weight > 0) {
                tf.setWeight(weight);
            }
            return tf;
        }
    }

    return {};
}

QLatin1String QTypeFace::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_TypeFace);
}