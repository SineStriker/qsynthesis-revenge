//
// Created by fluty on 2023/8/7.
//

#include "PanSlider.h"
#include "QAbstractSlider"
#include "QStyleOptionSlider"
#include "QPainter"
#include "QDebug"
#include "QTimer"

class PanSliderPrivate {
public:
    PanSlider *q;

    explicit PanSliderPrivate(PanSlider *q) : q(q) {
        q->setMinimumHeight(24);
        q->setMaximumHeight(24);
    }
};

PanSlider::PanSlider(QWidget *parent) : QWidget(parent), d(new PanSliderPrivate(this)) {
    timer = new QTimer(parent);
    timer->setInterval(200);
    QObject::connect(timer, &QTimer::timeout, this, [=]() {
        timer->stop();
        doubleClickLocked = false;
    });
}

PanSlider::~PanSlider() {
    delete d;
}

void PanSlider::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // Fill background
    // painter.fillRect(rect(), QColor("#d9d9d9"));

    // Calculate slider track inactive(background)
    actualLeft = rect().left() + 16;
    actualRight = rect().right() - 16;
    actualWidth = rect().width() - 16 - 16;
    auto trackLeft = QPoint(actualLeft, rect().height() / 2);
    auto trackRight = QPoint(actualRight, rect().height() / 2);

    // Draw slider track inactive(background)
    QPen pen;
    pen.setColor(QColor(217, 217, 217));
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(8);
    painter.setPen(pen);
    painter.drawLine(trackLeft, trackRight);

    // Calculate slider track active
    auto activeStartPos = actualWidth * (m_trackActiveStartValue - m_min) / (m_max - m_min) + 16;
    auto activePoint1 = QPoint(activeStartPos, rect().height() / 2);
    auto valuePos = actualWidth * (m_value - m_min) / (m_max - m_min) + 16;
    auto activePoint2 = QPoint(valuePos, rect().height() / 2);

    // Draw slider track active
    pen.setColor(QColor(112, 156, 255));
    pen.setWidth(8);
    painter.setPen(pen);
    painter.drawLine(activePoint1, activePoint2);

    // Draw handle
//    auto handleTopLeft = QPoint(rect().width() / 2 + rect().width() / 3 - 8, rect().height() / 2 - 8);
//    auto handleBottomRight = QPoint(rect().width() / 2 + rect().width() / 3 + 8, rect().height() / 2 + 8);
    auto handlePos = activePoint2;
    auto handleR = 8;
    pen.setColor(QColor(255, 255, 255));
    pen.setWidth(4);
    painter.setPen(pen);
    painter.setBrush(QColor(112, 156, 255));
    painter.drawEllipse(handlePos, handleR, handleR);

    painter.end();
}

void PanSlider::setValue(double value) {
    m_value = value;
    repaint();
}

void PanSlider::setDefaultValue(double value) {
    m_defaultValue = value;
    update();
}

void PanSlider::setMax(double max) {
    m_max = max;
    update();
}

void PanSlider::setMin(double min) {
    m_min = min;
    update();
}

void PanSlider::reset() {
    setValue(m_defaultValue);
}
void PanSlider::mouseMoveEvent(QMouseEvent *event) {
    auto pos = event->pos();
    if (pos.x() >= actualLeft && pos.x() <= actualRight) {
        //        auto valuePos = actualWidth * (m_value - m_min) / (m_max - m_min) + 16;
        auto posValue = (pos.x() - 16) * (m_max - m_min) / actualWidth + m_min;
        setValue(posValue);
    }
    QWidget::mouseMoveEvent(event);
}
void PanSlider::mouseDoubleClickEvent(QMouseEvent *event) {
//    qDebug() << "double click";
    auto pos = event->pos();
    if (MouseOnHandle(pos))
        reset();
    QWidget::mouseDoubleClickEvent(event);
}
void PanSlider::mousePressEvent(QMouseEvent *event) {
//    qDebug() << "press";
    timer->start();
    auto pos = event->pos();
    if (!MouseOnHandle(pos) && !doubleClickLocked) {
        if (pos.x() >= actualLeft && pos.x() <= actualRight) {
            //        auto valuePos = actualWidth * (m_value - m_min) / (m_max - m_min) + 16;
            auto posValue = (pos.x() - 16) * (m_max - m_min) / actualWidth + m_min;
            setValue(posValue);
        }
    }
    doubleClickLocked = true;
    QWidget::mousePressEvent(event);
}
void PanSlider::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
}
bool PanSlider::MouseOnHandle(const QPoint &mousePos) {
    auto pos = mousePos;
    auto valuePos = actualWidth * (m_value - m_min) / (m_max - m_min) + 16;
    if (pos.x() >= valuePos - 8 - 4 && pos.x() <= valuePos + 8 + 4) {
        return true;
    }
    return false;
}
void PanSlider::setTrackActiveStartValue(double pos) {
    m_trackActiveStartValue = pos;
    update();
}
