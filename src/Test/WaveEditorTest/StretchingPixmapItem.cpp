
#include <QPainter>
#include "StretchingPixmapItem.h"

StretchingPixmapItem::StretchingPixmapItem(QGraphicsItem *parent)
    : EventCaptureRectItem(parent)
{
}

StretchingPixmapItem::~StretchingPixmapItem()
{
}

void StretchingPixmapItem::SetPixmap(const QPixmap &pixmap)
{
    mPixmap = pixmap;
    setRect(pos().x(), pos().y(), pixmap.width(), pixmap.height());
}

void StretchingPixmapItem::SetPixmap(const QImage &image)
{
    mPixmap = QPixmap::fromImage(image);
    setRect(pos().x(), pos().y(), image.width(), image.height());
}

void StretchingPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(rect().toRect(), mPixmap);
}

void StretchingPixmapItem::eventSlot(QEvent *e, QPointF itemPos)
{
}
