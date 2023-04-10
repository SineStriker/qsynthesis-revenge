#include "NoneScrollArea.h"

#include <QDebug>

NoneScrollArea::NoneScrollArea(QWidget *parent) : QFrame(parent), m_widget(nullptr) {
    setAttribute(Qt::WA_StyledBackground);
}

NoneScrollArea::~NoneScrollArea() {
}

void NoneScrollArea::setWidget(QWidget *widget) {
    m_widget = widget;
    widget->setParent(this);
    widget->move(0, 0);
}

QWidget *NoneScrollArea::widget() const {
    return m_widget;
}

QWidget *NoneScrollArea::takeWidget() {
    m_widget = nullptr;
    return m_widget;
}

void NoneScrollArea::setPercentageX(double value) {
    if (!m_widget || m_widget->width() <= this->width()) {
        return;
    }
    double toX = (value / 100.0) * (this->width() - m_widget->width());
    m_widget->move(toX, m_widget->y());
}

void NoneScrollArea::setPercentageY(double value) {
    if (!m_widget || m_widget->height() <= this->height()) {
        return;
    }
    double toY = (value / 100.0) * (this->height() - m_widget->height());
    m_widget->move(m_widget->x(), toY);
}

double NoneScrollArea::percentageX() {
    if (!m_widget || m_widget->width() <= this->width()) {
        return 0;
    }
    return m_widget->x() / (this->width() - m_widget->width()) * 100.0;
}

double NoneScrollArea::percentageY() {
    if (!m_widget || m_widget->height() <= this->height()) {
        return 0;
    }
    return m_widget->y() / (this->height() - m_widget->height()) * 100.0;
}

void NoneScrollArea::setValueX(int value) {
    m_widget->move(-value, m_widget->y());
}

int NoneScrollArea::valueX() const {
    return -m_widget->x();
}

void NoneScrollArea::setValueY(int value) {
    m_widget->move(m_widget->x(), -value);
}

int NoneScrollArea::valueY() const {
    return -m_widget->y();
}

void NoneScrollArea::resizeEvent(QResizeEvent *event) {
    emit resized(event);
    QWidget::resizeEvent(event);
}
