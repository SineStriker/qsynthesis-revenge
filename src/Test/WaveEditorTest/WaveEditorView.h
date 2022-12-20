
#pragma once

#include <QGraphicsView>

class WaveEditorView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit WaveEditorView(QWidget *parent = nullptr);
    ~WaveEditorView();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

signals:
    void Resized();

    void RequestZoom(double zoomDelta, QPoint zoomAnchorGlobal);
};
