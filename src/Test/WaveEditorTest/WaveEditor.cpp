#include "WaveEditor.h"

#include "Common/CodecArguments.h"
#include "Common/SampleFormat.h"

#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QScrollBar>

WaveEditor::WaveEditor(QWidget *parent)
    : QObject(parent)
{
    mSampleCount = 0;

    mScene = new WaveEditorScene(this);
    mView = new WaveEditorView(parent);
    mView->setScene(mScene);
    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mScene->addItem(&mOverviewThumbnailItem);
    mScene->addItem(&mTimeAxisItem);
    mViewZoomLevel = 1;

    mOverviewThumbnailItem.setPos(0, 0);
    mOverviewThumbnailItem.setZValue(ZValue_OverviewThumbnail);

    mTimeAxisItem.setZValue(ZValue_TimeAxis);

    initPlugins();

    mRenderThread = nullptr;
    mRenderRoutineCommand = RENDER_IDLE;
    
    mRenderThread = new std::thread(&WaveEditor::renderRoutine, this);

    connect(mView, &WaveEditorView::Resized, this, &WaveEditor::WaveformViewResized);
    connect(mView, &WaveEditorView::RequestZoom, this, &WaveEditor::WaveformViewZoomRequested);
}

WaveEditor::~WaveEditor()
{
    mRenderRoutineCommand = RENDER_THREAD_EXIT;
    mRenderRoutineNotifyCond.notify_all();
    if (mRenderThread) {
        mRenderThread->join();
        delete mRenderThread;
        mRenderThread = nullptr;
    }

    uninitPlugins();
}

void WaveEditor::WaveformViewResized()
{
    // Get view size
    auto viewSize = mView->viewport()->size();

    // Stretch thumbnail item if view is larger than thumbnail
    auto rectStretchThumbnail = mOverviewThumbnailItem.rect();
    if(viewSize.width() > mOverviewThumbnail.width())
        rectStretchThumbnail.setWidth(viewSize.width());

    rectStretchThumbnail.setHeight(viewSize.height());

    mOverviewThumbnailItem.setRect(rectStretchThumbnail);
    mTimeAxisItem.setRect(0, 0, rectStretchThumbnail.width(), 40);
    mTimeAxisItem.SetThumbnailWidth(rectStretchThumbnail.width());
}

void WaveEditor::WaveformViewZoomRequested(double delta, QPoint zoomAnchorGlobal)
{
    // Zoom the view, resize elements, and the most complicated thing is to ensure the point on the
    // waveform where the mouse hovered over when the zoom happened still stayed
    // where the cursor is.

    QPoint anchorViewPos = mView->mapFromGlobal(zoomAnchorGlobal);
    QPointF anchorScenePos = mView->mapToScene(anchorViewPos);

    int fromX = anchorViewPos.x();

    auto origWidth = mOverviewThumbnail.width() * mViewZoomLevel;

    mViewZoomLevel *= delta;

    if (mViewZoomLevel < 1)
        mViewZoomLevel = 1;

    auto totalWidth = mOverviewThumbnail.width() * mViewZoomLevel;
    auto viewWidth = mView->viewport()->width();
    if (totalWidth < viewWidth)
        totalWidth = viewWidth;

    // Calculate new left-end
    int fromAbs = anchorScenePos.x() - (anchorViewPos.x() - fromX);
    int toX = fromX - fromAbs * totalWidth / origWidth;
    if (toX > 0)
        toX = 0;

    // Actually changing size here
    mOverviewThumbnailItem.setRect(0, 0, totalWidth, mOverviewThumbnailItem.rect().height());
    mTimeAxisItem.setRect(0, 0, totalWidth, mTimeAxisItem.rect().height());
    mView->setSceneRect(0, 0, totalWidth, mOverviewThumbnailItem.rect().height());

    mView->horizontalScrollBar()->setValue(-toX);
    qDebug() << toX;

    mView->update();
}

void WaveEditor::SetAudio(const QString &filename)
{
    if (!decoder->open({{QsMedia::KEY_NAME_FILE_NAME, filename},
                              {QsMedia::KEY_NAME_SAMPLE_FORMAT, QsMedia::AV_SAMPLE_FMT_FLT},
                              {QsMedia::KEY_NAME_CHANNELS, 1}})) {
        QMessageBox::critical(mView, qAppName(), "Failed to open audio file!");
        return;
    }

    mRenderRoutineCommand = RENDER_COARSE_PKPK;
    mRenderRoutineHaltFlag = 1;
    mRenderRoutineNotifyCond.notify_all();
}

void WaveEditor::renderRoutine()
{
    connect(this, &WaveEditor::renderRoutineFinishSignal,
            this, &WaveEditor::renderRoutineBackwardNotify,
            Qt::QueuedConnection);

    forever {
        // When there's HALT flag, means the loop isn't returning in a clean state
        // (i.e. previous command wasn't finished, otherwise halt flag is cleared)
        // so we have to skip the wait
        if(!mRenderRoutineHaltFlag) {
            std::unique_lock<std::mutex> uniqueLock(mRenderRoutineNotifyMutex);
            mRenderRoutineNotifyCond.wait(uniqueLock);
        }

        // Clear halt flag
        mRenderRoutineHaltFlag = 0;

        switch(mRenderRoutineCommand) {
            case RENDER_COARSE_PKPK:
            {
                decoder->Seek(0, WaveStream::Begin);
                mCoarsePkpk.clear();

                // Read data from decoder and find max-min values of each 100 sample slice
                float *buffer = new float[100];
                int read = 0;
                float saMax, saMin = saMax = 0.0f;

                // Performance profiling
                int pass = 0;
                double accumReadTime = 0.0, accumProcessTime = 0.0;

                do {
                    read = decoder->Read(buffer, 0, 100);
                    mSampleCount += read;

                    saMin = saMax = 0.0f;
                    for(int i = 0; i < read; i += 1) {
                        if (buffer[i] > saMax)
                            saMax = buffer[i];
                        if (buffer[i] < saMin)
                            saMin = buffer[i];
                    }
                    mCoarsePkpk.push_back({saMax, saMin});

                    if(mRenderRoutineHaltFlag) {
                        delete[] buffer;
                        goto Exit_Switch;
                    }
                } while (read > 0);
                read++; // Last read returns -1

                delete[] buffer;
                mRenderRoutineHaltFlag = 0;
                emit renderRoutineFinishSignal();
                break;
            }

            case RENDER_OVERVIEW_THUMBNAIL:
            {
                auto length = decoder->Length();
                decoder->Seek(0, WaveStream::Begin);

                // Active screen resolution / 2
                int width = QGuiApplication::primaryScreen()->geometry().width() / 2;
                float readSize = (float)mCoarsePkpk.size() / width;
                float *buffer = new float[readSize];

                QPixmap pixmap(width, 301);
                QPainter painter(&pixmap);

                int from = 0, to = 0;
                painter.setPen(QPen(Qt::red, 1));
                pixmap.fill(Qt::black);
                for(int i = 0; i < width; i++) {
                    // Draw waveform from the coarse pkpk data
                    float saMax = 0.0f, saMin = 0.0f;
                    from = round(i * readSize);
                    to = std::min((int)round((i + 1) * readSize), mCoarsePkpk.size());;
                    for(int j = from; j < to; j++) {
                        if (mCoarsePkpk[j].first > saMax)
                            saMax = mCoarsePkpk[j].first;
                        if (mCoarsePkpk[j].second < saMin)
                            saMin = mCoarsePkpk[j].second;
                    }
                    painter.drawLine(i, 150 - saMax * 150, i, 150 - saMin * 150);

                    if(mRenderRoutineHaltFlag) {
                        delete[] buffer;
                        goto Exit_Switch;
                    }
                }
                painter.end();

                delete[] buffer;
                mOverviewThumbnail = pixmap;
                mRenderRoutineHaltFlag = 0;
                emit renderRoutineFinishSignal();
                break;
            }
            case RENDER_VISIBLE_WAVEFORM:
            case RENDER_CACHE_WAVEFORM:
            case RENDER_WAVEFORM_DIRECT:
            case RENDER_IDLE:
                break;

            case RENDER_THREAD_EXIT:
                return;
        }
Exit_Switch:
        ;
    }
}

void WaveEditor::renderRoutineBackwardNotify() {
    switch(mRenderRoutineCommand) {
        case RENDER_COARSE_PKPK:
            mRenderRoutineCommand = RENDER_OVERVIEW_THUMBNAIL;
            mRenderRoutineNotifyCond.notify_all();
            qInfo() << "Coarse pkpk rendered";
            break;

        case RENDER_OVERVIEW_THUMBNAIL:
            mRenderRoutineCommand = RENDER_IDLE;
            mOverviewThumbnailItem.SetPixmap(mOverviewThumbnail);
            mView->resize(mOverviewThumbnail.width(), mOverviewThumbnail.height());
            auto sceneRect = mOverviewThumbnailItem.rect();
            sceneRect.setHeight(1);
            mView->setSceneRect(sceneRect);

            mTimeAxisItem.SetTimeScale(mSampleCount, decoder->outFormat().SampleRate(), mOverviewThumbnail.width());
            mTimeAxisItem.setRect(0, 0, mOverviewThumbnail.width(), 40);
            qInfo() << "Overview thumbnail rendered";
            break;
    }
}


void WaveEditor::initPlugins()
{
#ifdef Q_OS_LINUX
    decoderLoader.setFileName("audiodecoders/libFFmpegDecoder");
#else
    decoderLoader.setFileName("audiodecoders/FFmpegDecoder");
#endif

    decoder = qobject_cast<IAudioDecoder *>(decoderLoader.instance());

    if (!decoder) {
        QMessageBox::critical(
            nullptr, qAppName(),
            QString("Failed to load plugins: %1!").arg(decoderLoader.errorString()));
        goto out;
    }
    return;

out:
    uninitPlugins();
    ::exit(-1);
}

void WaveEditor::uninitPlugins()
{
    delete decoder;
    decoderLoader.unload();
}

