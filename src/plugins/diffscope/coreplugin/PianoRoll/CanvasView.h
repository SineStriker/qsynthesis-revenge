#ifndef CHORUSKIT_CANVASVIEW_H
#define CHORUSKIT_CANVASVIEW_H

#include <CGraphicsView.h>

#include "CanvasScene.h"

namespace Core {

    class CanvasViewPrivate;

    class CORE_EXPORT CanvasView : public CGraphicsView {
        Q_OBJECT
    public:
        explicit CanvasView(QWidget *parent = nullptr);
        ~CanvasView();

    public:
        CanvasScene *scene() const;

    private:
        CanvasViewPrivate *d;
    };

}



#endif // CHORUSKIT_CANVASVIEW_H
