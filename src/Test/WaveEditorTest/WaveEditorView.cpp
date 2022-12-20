
#include "WaveEditorView.h"
#include "EventCapture.h"
#include <QEvent>
#include <QMouseEvent>

WaveEditorView::WaveEditorView(QWidget *parent) : QGraphicsView(parent) {
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::Antialiasing, true);
}

WaveEditorView::~WaveEditorView() {
}

void WaveEditorView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    emit Resized();
}

bool WaveEditorView::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            auto pos = mouseEvent->localPos();
            auto item = static_cast<EventCaptureRectItem *>(itemAt(pos.x(), pos.y()));
            if (item)
                item->eventSlot(event, mapToScene(QPoint(pos.x(), pos.y())));
            break;
        }

        case QEvent::Wheel: {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
            auto pos = wheelEvent->position();
            auto item = static_cast<EventCaptureRectItem *>(itemAt(pos.x(), pos.y()));
            if (item)
                item->eventSlot(event, mapToScene(QPoint(pos.x(), pos.y())));
            break;
        }

        default:
            break;
    }
    return QGraphicsView::event(event);
}
