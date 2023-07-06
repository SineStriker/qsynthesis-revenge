#ifndef CANVASSCENE_H
#define CANVASSCENE_H

#include <CGraphicsScene.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class CanvasScenePrivate;

    class CORE_EXPORT CanvasScene : public CGraphicsScene {
        Q_OBJECT
        Q_DECLARE_PRIVATE(CanvasScene)
    public:
        explicit CanvasScene(QObject *parent = nullptr);
        ~CanvasScene();

    protected:
        CanvasScene(CanvasScenePrivate &d, QObject *parent = nullptr);

        QScopedPointer<CanvasScenePrivate> d_ptr;
    };

}

#endif // CANVASSCENE_H