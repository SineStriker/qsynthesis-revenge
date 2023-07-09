#ifndef CANVASSCENE_H
#define CANVASSCENE_H

#include <CGraphicsScene.h>

#include "coreplugin/CoreGlobal.h"
#include "coreplugin/Interfaces/IPianoRollComponent.h"

namespace Core {

    class CanvasScenePrivate;

    class CORE_EXPORT CanvasScene : public CGraphicsScene, public IPianoRollComponent {
        Q_OBJECT
        Q_DECLARE_PRIVATE(CanvasScene)
    public:
        explicit CanvasScene(IProjectWindow *iWin, QObject *parent = nullptr);
        ~CanvasScene();

        void initialize() override;
        void extensionInitialized() override;

    signals:
        void viewMoved(const QPointF &pos, const QPointF &oldPos);
        void viewResized(const QSizeF &size, const QSizeF &newSize);

    protected:
        void viewMoveEvent(QGraphicsSceneMoveEvent *event) override;
        void viewResizeEvent(QGraphicsSceneResizeEvent *event) override;

        void drawBackground(QPainter *painter, const QRectF &rect) override;

    protected:
        CanvasScene(CanvasScenePrivate &d, IProjectWindow *iWin, QObject *parent = nullptr);

        QScopedPointer<CanvasScenePrivate> d_ptr;
    };

}

#endif // CANVASSCENE_H