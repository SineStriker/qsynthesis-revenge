#include "CanvasScene.h"
#include "CanvasScene_p.h"

#include "Window/IProjectWindow.h"

#include <QDebug>

namespace Core {

    CanvasScenePrivate::CanvasScenePrivate() {
        timeMgr = nullptr;
    }

    CanvasScenePrivate::~CanvasScenePrivate() {
    }

    void CanvasScenePrivate::init() {
    }

    void CanvasScenePrivate::adjustSceneRect() {
        Q_Q(CanvasScene);
        q->setSceneRect(QRect(0, 0, 10000, timeMgr->currentHeight() * 84));
    }

    void CanvasScenePrivate::_q_currentWidthChanged(int w) {
        adjustSceneRect();
    }

    void CanvasScenePrivate::_q_currentHeightChanged(int h) {
        adjustSceneRect();
    }

    void CanvasScenePrivate::_q_currentSnapChanged(int s) {
    }

    CanvasScene::CanvasScene(IProjectWindow *iWin, QObject *parent)
        : CanvasScene(*new CanvasScenePrivate(), iWin, parent) {
    }

    CanvasScene::~CanvasScene() {
    }

    void CanvasScene::initialize() {
        Q_D(CanvasScene);

        auto timeMgr = iWin->timeManager();
        d->timeMgr = timeMgr;

        connect(timeMgr, &MusicTimeManager::currentWidthChanged, d, &CanvasScenePrivate::_q_currentWidthChanged);
        connect(timeMgr, &MusicTimeManager::currentHeightChanged, d, &CanvasScenePrivate::_q_currentHeightChanged);
        connect(timeMgr, &MusicTimeManager::currentSnapChanged, d, &CanvasScenePrivate::_q_currentSnapChanged);
    }

    void CanvasScene::extensionInitialized() {
        Q_D(CanvasScene);
        d->adjustSceneRect();
    }

    void CanvasScene::viewMoveEvent(QGraphicsSceneMoveEvent *event) {
        emit viewMoved(event->newPos(), event->oldPos());
    }

    void CanvasScene::viewResizeEvent(QGraphicsSceneResizeEvent *event) {
        emit viewResized(event->newSize(), event->oldSize());
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
