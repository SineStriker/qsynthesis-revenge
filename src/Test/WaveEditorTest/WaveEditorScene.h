
#pragma once

#include <QGraphicsScene>

class WaveEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit WaveEditorScene(QObject *parent = nullptr);
    ~WaveEditorScene();

protected:
    // bool event(QEvent *event) override;

};
