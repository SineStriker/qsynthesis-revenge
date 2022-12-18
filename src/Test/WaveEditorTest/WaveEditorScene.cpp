
#include "WaveEditorScene.h"

WaveEditorScene::WaveEditorScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(Qt::DiagCrossPattern));
}

WaveEditorScene::~WaveEditorScene()
{
}
