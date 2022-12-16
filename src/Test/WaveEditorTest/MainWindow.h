
#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPluginLoader>
#include <QGraphicsPixmapItem>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Api/IAudioDecoder.h"
#include "WaveEditorScene.h"
#include "WaveEditorView.h"

class RenderRoutineBackwardNotifier : public QObject {
        Q_OBJECT
    public:
        RenderRoutineBackwardNotifier(QObject *parent) : QObject(parent) {}
    signals:
        void notify();
};

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

    WaveEditorScene *mScene;
    WaveEditorView *mView;

    QPluginLoader decoderLoader;
    IAudioDecoder *decoder;

    QGraphicsPixmapItem mOverviewThumbnailItem;

// Waveform rendering related members
    // The min-max pairs in each 100 samples range
    QVector<QPair<float, float>> mCoarsePkpk;

    // Overview thumbnail generated based on user's monitor resolution.
    // I say we use (horizontal res / 2) as the horizontal resolution for now
    // Use 301 pixels for vertical resolution should be more than enough
    // Kinda a hack, but that's all I have for now.
    QPixmap mOverviewThumbnail;

    // Cached waveform based on coarse pkpks
    // Each image is 0.5s of audio
    QList<QImage> mWaveformCache;

    // Render thread, occupied by rendering routines but only one at a time.
    // 1 - generate the coarse pkpk data and the overview thumbnail (same time)
    // 2 - generate the waveform image when user is zooming in enough, and cache the rest of audio
    //     cache happens when it's idle and should be able to be halted at any time
    // 3 - generate waveform directly on original samples when zoomed in very close
    std::thread *mRenderThread;

    enum {
        RENDER_IDLE = 0,
        RENDER_COARSE_PKPK,
        RENDER_OVERVIEW_THUMBNAIL,
        RENDER_VISIBLE_WAVEFORM,
        RENDER_CACHE_WAVEFORM,
        RENDER_WAVEFORM_DIRECT,

        RENDER_THREAD_EXIT,

        // HALT flag. If the flag is present when entering the render thread main loop,
        // means the thread has not finished previously assigned task (it has been halted)
    };
    std::atomic_int mRenderRoutineCommand;
    volatile int mRenderRoutineHaltFlag;

    std::condition_variable mRenderRoutineNotifyCond;
    std::mutex mRenderRoutineNotifyMutex;

    RenderRoutineBackwardNotifier *mRenderRoutineBackwardNotifier;

// Waveform rendering routines
    void renderRoutine();

private slots:
    void renderRoutineBackwardNotify();

signals:
    void renderRoutineFinishSignal();

private:
    void initPlugins();
    void uninitPlugins();
};
