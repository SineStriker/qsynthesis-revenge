#include "CanvasScene.h"
#include "CanvasScene_p.h"

namespace Core {

CanvasScenePrivate::CanvasScenePrivate() {
}

CanvasScenePrivate::~CanvasScenePrivate() {
}

void CanvasScenePrivate::init() {
}

CanvasScene::CanvasScene(QObject *parent) : CanvasScene(*new CanvasScenePrivate(), parent) {
}

CanvasScene::~CanvasScene() {
}

CanvasScene::CanvasScene(CanvasScenePrivate &d, QObject *parent) : CGraphicsScene(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

}
