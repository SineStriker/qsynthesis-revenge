
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

signals:
    void Resized();
};
