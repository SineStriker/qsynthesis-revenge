
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include "EventCapture.h"
#include "WaveEditorScene.h"

WaveEditorScene::WaveEditorScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(Qt::DiagCrossPattern));
}

WaveEditorScene::~WaveEditorScene()
{
}

// bool WaveEditorScene::event(QEvent *event)
// {
//     switch(event->type())
//     {
//         case QEvent::GraphicsSceneMousePress: {
//             QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
//             auto pos = mouseEvent->scenePos();
//             static_cast<EventCaptureRectItem*>(itemAt(pos.x(), pos.y()))->eventSlot(event);
//             break;
//         }
//     }
//     return QGraphicsView::event(event);
// }
