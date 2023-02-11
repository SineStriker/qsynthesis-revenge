#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPluginLoader>
#include <QPushButton>
#include <QSet>
#include <QSlider>

#include "Api/IAudioDecoder.h"
#include "Api/IAudioPlayback.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QPluginLoader decoderLoader, playbackLoader;

    IAudioDecoder *decoder;
    IAudioPlayback *playback;

    QMenu *fileMenu;
    QAction *browseAction;

    QMenu *deviceMenu;
    QActionGroup *deviceActionGroup;

    QLabel *fileLabel;
    QSlider *slider;
    QLabel *timeLabel;
    QPushButton *playButton;
    QPushButton *stopButton;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonsLayout;

    int notifyTimerId;
    bool playing;
    QString filename;
    QSet<QString> filters;

    void openFile(const QString &filename);
    void setPlaying(bool playing);

    void timerEvent(QTimerEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void initPlugins();
    void initStyleSheet();

    void uninitPlugins();

    void reloadDevices();
    void reloadButtonStatus();
    void reloadSliderStatus();
    void reloadDeviceActionStatus();

    void _q_browseActionTriggered();
    void _q_playButtonClicked();
    void _q_stopButtonClicked();
    void _q_sliderReleased();
    void _q_deviceActionTriggered(QAction *action);
    void _q_playStateChanged();
    void _q_audioDeviceChanged();
    void _q_audioDeviceAdded();
    void _q_audioDeviceRemoved();
};

#endif // MAINWINDOW_H
