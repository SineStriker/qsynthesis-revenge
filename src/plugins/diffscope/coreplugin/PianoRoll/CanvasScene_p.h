#ifndef CANVASSCENEPRIVATE_H
#define CANVASSCENEPRIVATE_H

#include "CanvasScene.h"

namespace Core {

class CanvasScenePrivate {
    Q_DECLARE_PUBLIC(CanvasScene)
public:
    CanvasScenePrivate();
    virtual ~CanvasScenePrivate();

    void init();

    CanvasScene *q_ptr;
};

}

#endif // CANVASSCENEPRIVATE_H