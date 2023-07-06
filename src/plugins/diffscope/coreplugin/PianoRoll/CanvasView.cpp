#include "CanvasView.h"

namespace Core {

    class CanvasViewPrivate {
    public:
        CanvasViewPrivate(CanvasView *q) : q(q) {
        }

        CanvasView *q;
    };

    CanvasView::CanvasView(QWidget *parent) : CGraphicsView(parent), d(new CanvasViewPrivate(this)) {
    }

    CanvasView::~CanvasView() {
        delete d;
    }

    CanvasScene *CanvasView::scene() const {
        return static_cast<CanvasScene *>(CGraphicsView::scene());
    }

}