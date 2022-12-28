#pragma once

#include <QGraphicsPixmapItem>
#include <QPluginLoader>

#include <condition_variable>
#include <mutex>
#include <thread>

#include "Api/IAudioDecoder.h"
#include "StretchingPixmapItem.h"
#include "TimeAxisItem.h"
#include "WaveformItem.h"
#include "WaveEditorScene.h"
#include "WaveEditorView.h"

class WaveEditor : public QObject {
    Q_OBJECT
public:
    WaveEditor(QWidget *parent);
    ~WaveEditor();

    void SetAudio(const QString &filename);

    inline QWidget *view() const {
        return mView;
    }

private:
    WaveEditorScene *mScene;
    WaveEditorView *mView;

    QPluginLoader decoderLoader;
    IAudioDecoder *decoder;

    uint64_t mSampleCount;

    // TODO: New documentation on this LOD system
    StretchingPixmapItem mOverviewThumbnailItem;
    TimeAxisItem mTimeAxisItem;
    WaveformItem *mWaveItemCoarse1, *mWaveItemDirect;

    /*
        -3: 1sa/pt (direct)
        -2: 4sa/pt (skip)
        -1: 20sa/pt (skip)
        0: 100sa/pt (cache)
        1: 500sa/pt (cache)
        2: 2500sa/pt
        and so on
    */
    int mCurrentLod;

    // Waveform rendering related members
    // The min-max pairs in each 100 samples range
    QVector<QPair<float, float>> mCoarse1Pkpk;
    // In each 0.1s, for long audio >= 10min
    QVector<QPair<float, float>> mCoarse2Pkpk;

    // 0: 100sa/pt 1: 500sa/pt 2:2500sa/pt on and on...
    QVector<QVector<QPair<float, float>>> mCachedLodPkpks;

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
    void WaveformViewZoomRequested(double zoomDelta, QPoint zoomAnchorGlobal);

private:
    double mViewZoomLevel;
    uint64_t mViewStartSample;

private:
    void initPlugins();
    void uninitPlugins();

    void DetermineLod();
    void SetLod(int);
};
