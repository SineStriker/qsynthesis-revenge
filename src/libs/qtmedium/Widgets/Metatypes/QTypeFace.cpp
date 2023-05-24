#include "QTypeFace.h"

#include "QMCss.h"
#include "private/QMetaTypeUtils.h"

#include <private/qfont_p.h>

#include <QApplication>

QTypeFace::QTypeFace() {
    init();
}

QTypeFace::QTypeFace(const QColor &color, int pixelSize) {
    init();
    setColor(color);
    setPixelSize(pixelSize);
}

QTypeFace::QTypeFace(const QColor &color, double pointSize) {
    init();
    setColor(color);
    setPointSize(pointSize);
}

QTypeFace::QTypeFace(const QList<QColor> &colors, int pixelSize) {
    init();
    setColors(colors);
    setPixelSize(pixelSize);
}

QTypeFace::QTypeFace(const QList<QColor> &colors, double pointSize) {
    init();
    setColors(colors);
    setPointSize(pointSize);
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
                                    .arg(color().name(),
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

QList<QColor> QTypeFace::colors() const {
    return m_colors;
}

void QTypeFace::setColors(const QList<QColor> &colors) {
    m_colors = colors;
}

QColor QTypeFace::color() const {
    return m_colors.isEmpty() ? QColor() : m_colors.front();
}

QColor QTypeFace::color2() const {
    return m_colors.size() < 2 ? color() : m_colors.at(1);
}

QColor QTypeFace::color3() const {
    return m_colors.size() < 3 ? color2() : m_colors.at(2);
}

QColor QTypeFace::color4() const {
    return m_colors.size() < 4 ? color3() : m_colors.at(3);
}

QTypeFace QTypeFace::fromStringList(const QStringList &stringList) {
    if (stringList.size() == 2 && !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList content = SplitStringByComma(stringList.back());
        for (auto &item : content) {
            item = item.simplified();
        }
        if (!content.isEmpty()) {
            QTypeFace tf;

            QString strColor = content.front().simplified();
            if (strColor.startsWith('(') && strColor.endsWith(')')) {
                QStringList colorStrings = SplitStringByComma(strColor.mid(1, strColor.size() - 2));
                QList<QColor> colors;
                for (const auto &item : qAsConst(colorStrings)) {
                    colors.append(QMCss::CssStringToColor(item.simplified()));
                }
                tf.setColors(colors);
            } else {
                tf.setColors({QMCss::CssStringToColor(strColor)});
            }

            int pixelSize = -1;
            double pointSize = -1;
            int weight = -1;

            QLatin1String px(PixelSizeUnit);
            if (content.size() > 1) {
                QString strPixel = content.at(1);
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
                if (strWeight.endsWith(px, Qt::CaseInsensitive)) {
                    strWeight.chop(px.size());
                }

                bool isNum;
                int num = strWeight.toInt(&isNum);
                if (isNum) {
                    weight = num;
                }
            }
            if (pixelSize >= 0) {
                tf.setPixelSize(pixelSize);
            } else if (pointSize >= 0) {
                tf.setPointSize(pointSize);
            }
            if (weight >= 0) {
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
