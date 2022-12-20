
#pragma once

#include <QGraphicsRectItem>
#include <QEvent>

class EventCaptureRectItem : public QGraphicsRectItem {
public:
    EventCaptureRectItem(QGraphicsItem *parent = nullptr) : QGraphicsRectItem(parent) {};
    ~EventCaptureRectItem() {};

    virtual void eventSlot(QEvent *event, QPointF scenePos) = 0;
};
