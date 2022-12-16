
#include "WaveEditorView.h"

WaveEditorView::WaveEditorView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::Antialiasing, true);
}

WaveEditorView::~WaveEditorView()
{
}

