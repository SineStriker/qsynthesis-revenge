#ifndef CHORUSKIT_CANVASVIEW_H
#define CHORUSKIT_CANVASVIEW_H

#include <CGraphicsView.h>

#include "CanvasScene.h"

namespace Core {

    class CanvasViewPrivate;

    class CORE_EXPORT CanvasView : public CGraphicsView, public IPianoRollComponent {
        Q_OBJECT
    public:
        explicit CanvasView(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~CanvasView();

        void initialize() override;
        void extensionInitialized() override;

    public:
        CanvasScene *scene() const;

    private:
        CanvasViewPrivate *d;

        friend class CanvasViewPrivate;
    };

}



#endif // CHORUSKIT_CANVASVIEW_H
