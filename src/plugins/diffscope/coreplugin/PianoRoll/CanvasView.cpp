#include "CanvasView.h"

namespace Core {

    class CanvasViewPrivate {
    public:
        CanvasViewPrivate(CanvasView *q) : q(q) {
            scene = new CanvasScene(q->iWin, q);
            scene->setBackgroundBrush(Qt::blue);
            q->setScene(scene);
            q->setFrameStyle(QFrame::NoFrame);
        }

        CanvasView *q;

        CanvasScene *scene;
    };

    CanvasView::CanvasView(IProjectWindow *iWin, QWidget *parent)
        : CGraphicsView(parent), IPianoRollComponent(iWin), d(new CanvasViewPrivate(this)) {
    }

    CanvasView::~CanvasView() {
        delete d;
    }

    void CanvasView::initialize() {
        d->scene->initialize();
    }

    void CanvasView::extensionInitialized() {
        d->scene->extensionInitialized();
    }

    CanvasScene *CanvasView::scene() const {
        return d->scene;
    }

}