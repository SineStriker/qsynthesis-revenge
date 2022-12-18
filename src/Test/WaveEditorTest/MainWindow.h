
#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPluginLoader>
#include <QGraphicsPixmapItem>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Api/IAudioDecoder.h"
#include "WaveEditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetAudio(const QString &filename);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

protected:
    QVBoxLayout *mLayMain;
    WaveEditor *mWaveEditor;

};
