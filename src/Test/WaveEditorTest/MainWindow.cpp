
#include "MainWindow.h"
#include "Common/CodecArguments.h"
#include "Common/SampleFormat.h"
#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <atomic>
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mScene = new WaveEditorScene(this);
    mView = new WaveEditorView(this);
    mView->setScene(mScene);
    setCentralWidget(mView);

    mScene->addItem(&mOverviewThumbnailItem);
    mOverviewThumbnailItem.setPos(0, 0);

    initPlugins();

    mRenderThread = nullptr;
    mRenderRoutineCommand = RENDER_IDLE;
    mRenderRoutineBackwardNotifier = nullptr;
    
    mRenderThread = new std::thread(&MainWindow::renderRoutine, this);
}

MainWindow::~MainWindow()
{
    mRenderRoutineCommand = RENDER_HALT_CURRENT | RENDER_THREAD_EXIT;
    mRenderRoutineNotifyCond.notify_all();
    if (mRenderThread) {
        mRenderThread->join();
        delete mRenderThread;
    }

    uninitPlugins();
}

void MainWindow::SetAudio(const QString &filename)
{
    if (!decoder->open({{QsMedia::KEY_NAME_FILE_NAME, filename},
                              {QsMedia::KEY_NAME_SAMPLE_FORMAT, QsMedia::AV_SAMPLE_FMT_FLT}})) {
        QMessageBox::critical(this, qAppName(), "Failed to open audio file!");
        return;
    }

    qInfo() << "Samples: " << decoder->Length();

    mRenderRoutineCommand = RENDER_COARSE_PKPK | RENDER_HALT_CURRENT;
    mRenderRoutineNotifyCond.notify_all();
}

void MainWindow::renderRoutine()
{
    connect(this, &MainWindow::renderRoutineFinishSignal,
            this, &MainWindow::renderRoutineBackwardNotify,
            Qt::QueuedConnection);

    forever {
        // When there's HALT flag, means the loop isn't returning in a clean state
        // (i.e. previous command wasn't finished, otherwise halt flag is cleared)
        // so we have to skip the wait
        if(!(mRenderRoutineCommand & RENDER_HALT_CURRENT)) {
            std::unique_lock<std::mutex> uniqueLock(mRenderRoutineNotifyMutex);
            mRenderRoutineNotifyCond.wait(uniqueLock);
        }

        // Clear halt flag
        mRenderRoutineCommand &= ~RENDER_HALT_CURRENT;

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
                    std::cerr << std::fixed << std::setprecision(9) << std::left;
                    // std::cerr << "Pass " << pass++ << '\n';
                    auto start = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> diff;

                    read = decoder->Read(buffer, 0, 100);

                    auto end = std::chrono::high_resolution_clock::now();
                    diff = end - start;
                    // std::cerr << "        Read time: " << diff.count() << "s\n";
                    accumReadTime += diff.count();
                    start = end;

                    saMin = saMax = 0.0f;
                    for(int i = 0; i < read; i += 1) {
                        if (buffer[i] > saMax)
                            saMax = buffer[i];
                        if (buffer[i] < saMin)
                            saMin = buffer[i];
                    }
                    mCoarsePkpk.push_back({saMax, saMin});

                    end = std::chrono::high_resolution_clock::now();
                    diff = end - start;
                    // std::cerr << std::setw(9) << "        Process time: " << diff.count() << "ms\n";
                    accumProcessTime += diff.count();

                    if(mRenderRoutineCommand & RENDER_HALT_CURRENT) {
                        delete[] buffer;
                        goto Exit_Switch;
                    }
                } while (read > 0);

                std::cerr << "Total pass count: " << pass   << '\n';
                std::cerr << "Read time: " << accumReadTime << "s\n";
                std::cerr << "Process time: " << accumProcessTime << "s\n";

                delete[] buffer;
                mRenderRoutineCommand &= ~RENDER_HALT_CURRENT;
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

                    if(mRenderRoutineCommand & RENDER_HALT_CURRENT) {
                        delete[] buffer;
                        goto Exit_Switch;
                    }
                }
                painter.end();

                delete[] buffer;
                mOverviewThumbnail = pixmap;
                mRenderRoutineCommand &= ~RENDER_HALT_CURRENT;
                emit renderRoutineFinishSignal();
                break;
            }
            case RENDER_VISIBLE_WAVEFORM:
            case RENDER_CACHE_WAVEFORM:
            case RENDER_WAVEFORM_DIRECT:
            case RENDER_HALT_CURRENT:
            case RENDER_IDLE:
                break;

            case RENDER_THREAD_EXIT:
                return;
        }
Exit_Switch:
        ;
    }
}

void MainWindow::renderRoutineBackwardNotify() {
    switch(mRenderRoutineCommand) {
        case RENDER_COARSE_PKPK:
            mRenderRoutineCommand = RENDER_OVERVIEW_THUMBNAIL;
            mRenderRoutineNotifyCond.notify_all();
            qInfo() << "Coarse pkpk rendered";
            break;

        case RENDER_OVERVIEW_THUMBNAIL:
            mRenderRoutineCommand = RENDER_IDLE;
            mOverviewThumbnailItem.setPixmap(mOverviewThumbnail);
            mView->resize(mOverviewThumbnail.width(), mOverviewThumbnail.height());
            mView->setSceneRect(mOverviewThumbnail.rect());
            this->resize(mOverviewThumbnail.width(), mOverviewThumbnail.height());
            qInfo() << "Overview thumbnail rendered";
            break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // mView->fitInView(mOverviewThumbnail.rect());
}

void MainWindow::initPlugins()
{
#ifdef Q_OS_LINUX
    decoderLoader.setFileName("resources/modules/audiodecoders/libFFmpegDecoder");
#else
    decoderLoader.setFileName("resources/modules/audiodecoders/FFmpegDecoder");
#endif

    decoder = qobject_cast<IAudioDecoder *>(decoderLoader.instance());

    if (!decoder) {
        QMessageBox::critical(
            this, qAppName(),
            QString("Failed to load plugins: %1!").arg(decoderLoader.errorString()));
        goto out;
    }
    return;

out:
    uninitPlugins();
    ::exit(-1);
}

void MainWindow::uninitPlugins()
{
    delete decoder;
    decoderLoader.unload();
}
