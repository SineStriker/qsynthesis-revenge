#include "CanvasScene.h"
#include "CanvasScene_p.h"

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

    CanvasScene::CanvasScene(CanvasScenePrivate &d, IProjectWindow *iWin, QObject *parent)
        : CGraphicsScene(parent), IPianoRollComponent(iWin), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
