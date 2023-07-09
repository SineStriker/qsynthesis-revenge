#ifndef CANVASSCENEPRIVATE_H
#define CANVASSCENEPRIVATE_H

#include "CanvasScene.h"

namespace Core {

    class MusicTimeManager;

    class CanvasScenePrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(CanvasScene)
    public:
        CanvasScenePrivate();
        virtual ~CanvasScenePrivate();

        void init();

        CanvasScene *q_ptr;

        MusicTimeManager *timeMgr;

        void adjustSceneRect();

    private:
        void _q_currentWidthChanged(int w);
        void _q_currentHeightChanged(int h);
        void _q_currentSnapChanged(int s);
    };

}

#endif // CANVASSCENEPRIVATE_H