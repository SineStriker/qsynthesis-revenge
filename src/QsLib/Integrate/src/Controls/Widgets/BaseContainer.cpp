#include "BaseContainer.h"

#include <QBrush>
#include <QDebug>
#include <QPainter>

BaseContainer::BaseContainer(QWidget *parent) : QFrame(parent) {
    setAttribute(Qt::WA_StyledBackground);

    m_wallpaper = Qt::transparent;
    m_borderColor = Qt::transparent;
}

BaseContainer::~BaseContainer() {
}

QMargins BaseContainer::margins() const {
    return m_margins;
}

void BaseContainer::setMargins(const QMargins &margins) {
    m_margins = margins;
    update();
    emit marginsChanged();
}

QColor BaseContainer::wallpaper() const {
    return m_wallpaper;
}

void BaseContainer::setWallpaper(const QColor &wallpaper) {
    m_wallpaper = wallpaper;
    update();
    emit colorChanged();
}

QColor BaseContainer::borderColor() const {
    return m_borderColor;
}

void BaseContainer::setBorderColor(const QColor &borderColor) {
    m_borderColor = borderColor;
    update();
    emit colorChanged();
}

void BaseContainer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(m_wallpaper);
    painter.setPen(Qt::NoPen);

    QRect rect = contentsRect();

    int left = m_margins.left();
    int right = m_margins.right();
    int top = m_margins.top();
    int bottom = m_margins.bottom();

    painter.drawRect(rect.adjusted(left, top, -right, -bottom));

    int w = rect.width();
    int h = rect.height();
    painter.setBrush(m_borderColor);
    painter.translate(rect.topLeft());

    if (left > 0) {
        painter.drawRect(0, 0, left, h);
    }
    if (right > 0) {
        painter.drawRect(w - right, 0, right, h);
    }
    if (top > 0) {
        painter.drawRect(left, 0, w - left - right, top);
    }
    if (bottom > 0) {
        painter.drawRect(left, h - bottom, w - left - right, bottom);
    }
}
