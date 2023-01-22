#include "CGraphicsScene.h"

CGraphicsScene::CGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
}

CGraphicsScene::CGraphicsScene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent) {
}

CGraphicsScene::CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent) {
}

CGraphicsScene::~CGraphicsScene() {
}

bool CGraphicsScene::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::GraphicsSceneMove:
        viewMoveEvent(static_cast<QGraphicsSceneMoveEvent *>(event));
        return event->isAccepted();
        break;
    case QEvent::GraphicsSceneResize:
        viewResizeEvent(static_cast<QGraphicsSceneResizeEvent *>(event));
        return event->isAccepted();
        break;

    case QEvent::Enter:
        enterEvent(static_cast<QEnterEvent *>(event));
        return event->isAccepted();
        break;
    case QEvent::Leave:
        leaveEvent(event);
        return event->isAccepted();
        break;
    default:
        break;
    }
    return QGraphicsScene::event(event);
}

void CGraphicsScene::viewMoveEvent(QGraphicsSceneMoveEvent *event) {
    Q_UNUSED(event)
}

void CGraphicsScene::viewResizeEvent(QGraphicsSceneResizeEvent *event) {
    Q_UNUSED(event)
}

void CGraphicsScene::enterEvent(QEnterEvent *event) {
    Q_UNUSED(event)
}

void CGraphicsScene::leaveEvent(QEvent *event) {
    Q_UNUSED(event)
}
