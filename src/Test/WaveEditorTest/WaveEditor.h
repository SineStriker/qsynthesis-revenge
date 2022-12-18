
#pragma once

#include <QPluginLoader>
#include <QGraphicsPixmapItem>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Api/IAudioDecoder.h"
#include "WaveEditorScene.h"
#include "WaveEditorView.h"
#include "StretchingPixmapItem.h"
#include "TimeAxisItem.h"

class WaveEditor : public QObject {
    Q_OBJECT
public:
    WaveEditor(QWidget *parent);
    ~WaveEditor();

    void SetAudio(const QString &filename);

    operator QWidget*() { return mView; }

private:
    WaveEditorScene *mScene;
    WaveEditorView *mView;

    QPluginLoader decoderLoader;
    IAudioDecoder *decoder;

    uint64_t mSampleCount;

    // There are multiple pixmap items that are displayed on the scene so let me introduce the
    // designs here first.
    // 1. Overview thumbnail. First rendered for entire audio and is always there as fallback.
    // 2. Coarse waveform. Rendered when zoomed in enough and are cached in 0.5s slices.
    //    Note that the thumbnail is still used when zoomed out.
    //    This LOD level of waveform is cached in background once triggered the first render.
    // 3. Fine waveform, Rendered when zoomed in further and samples are directly taken from decoder.
    //    This one is only one single pixmap item and is not cached.
    StretchingPixmapItem mOverviewThumbnailItem;

    TimeAxisItem mTimeAxisItem;

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
    };
    std::atomic_int mRenderRoutineCommand;
    // HALT flag. If the flag is present when entering the render thread main loop,
    // means the thread has not finished previously assigned task (it has been halted)
    volatile int mRenderRoutineHaltFlag;

    std::condition_variable mRenderRoutineNotifyCond;
    std::mutex mRenderRoutineNotifyMutex;
// End of waveform rendering related members

// Waveform rendering routines
    void renderRoutine();

private slots:
    void renderRoutineBackwardNotify();

signals:
    void renderRoutineFinishSignal();
// End of waveform rendering related members

public:
    enum ItemZValue {
        ZValue_OverviewThumbnail = 0,
        ZValue_CoarseWaveform,
        ZValue_FineWaveform,
        ZValue_TimeAxis,
        ZValue_Playhead,
    };

private slots:
    void WaveformViewResized();

private:
    void initPlugins();
    void uninitPlugins();
};
