
#pragma once

#include <QGraphicsRectItem>
#include "EventCapture.h"

class StretchingPixmapItem : public EventCaptureRectItem
{
public:
    StretchingPixmapItem(QGraphicsItem *parent = nullptr);
    ~StretchingPixmapItem();

    void SetPixmap(const QPixmap &pixmap);
    void SetPixmap(const QImage &image);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual void eventSlot(QEvent* e, QPointF itemPos) override;

private:
    QPixmap mPixmap;
};
