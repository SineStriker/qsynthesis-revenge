#include "CanvasScene.h"
#include "CanvasScene_p.h"

#include "Window/IProjectWindow.h"

#include <QDebug>

namespace Core {

    CanvasScenePrivate::CanvasScenePrivate() {
    }

    CanvasScenePrivate::~CanvasScenePrivate() {
    }

    void CanvasScenePrivate::init() {
    }

    CanvasScene::CanvasScene(IProjectWindow *iWin, QObject *parent)
        : CanvasScene(*new CanvasScenePrivate(), iWin, parent) {
    }

    CanvasScene::~CanvasScene() {
    }

    void CanvasScene::initialize() {
    }

    void CanvasScene::extensionInitialized() {
        setSceneRect(QRect(0, 0, 10000, 4000));
    }

    void CanvasScene::viewMoveEvent(QGraphicsSceneMoveEvent *event) {
        emit iWin->timeManager()->viewMoved(event->newPos(), event->oldPos());
    }

    void CanvasScene::viewResizeEvent(QGraphicsSceneResizeEvent *event) {
        emit iWin->timeManager()->viewResized(event->newSize(), event->oldSize());
    }

    void CanvasScene::drawBackground(QPainter *painter, const QRectF &rect) {
        QGraphicsScene::drawBackground(painter, rect);
    }

    CanvasScene::CanvasScene(CanvasScenePrivate &d, IProjectWindow *iWin, QObject *parent)
        : CGraphicsScene(parent), IPianoRollComponent(iWin), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
