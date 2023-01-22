#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPluginLoader>
#include <QTime>

#include "Common/CodecArguments.h"
#include "Common/SampleFormat.h"

#include "MathHelper.h"
#include "QsSystem.h"

// https://iconduck.com/icons

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    notifyTimerId = 0;
    playing = false;
    filters = QSet<QString>{"wav", "mp3", "m4a", "flac"};

    setAcceptDrops(true);

    initPlugins();
    initStyleSheet();

    // Init Menu Bar
    browseAction = new QAction("Open Audio", this);
    browseAction->setShortcut(QKeySequence("Ctrl+O"));

    fileMenu = new QMenu("File(&F)", this);
    fileMenu->addAction(browseAction);

    deviceMenu = new QMenu("Devices(&D)", this);
    deviceActionGroup = new QActionGroup(this);
    deviceActionGroup->setExclusive(true);

    auto bar = menuBar();
    bar->addMenu(fileMenu);
    bar->addMenu(deviceMenu);

    // Init Central
    fileLabel = new QLabel("Select audio file.");
    timeLabel = new QLabel("--:--/--:--");

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 10000);

    playButton = new QPushButton();
    playButton->setObjectName("play-button");

    stopButton = new QPushButton();
    stopButton->setObjectName("stop-button");

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(playButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(timeLabel);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(fileLabel);
    mainLayout->addWidget(slider);
    mainLayout->addLayout(buttonsLayout);

    auto w = new QFrame();
    w->setObjectName("central-widget");
    w->setLayout(mainLayout);

    setCentralWidget(w);

    connect(browseAction, &QAction::triggered, this, &MainWindow::_q_browseActionTriggered);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::_q_playButtonClicked);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::_q_stopButtonClicked);
    connect(slider, &QSlider::sliderReleased, this, &MainWindow::_q_sliderReleased);
    connect(deviceMenu, &QMenu::triggered, this, &MainWindow::_q_deviceActionTriggered);

    connect(playback, &IAudioPlayback::stateChanged, this, &MainWindow::_q_playStateChanged);
    connect(playback, &IAudioPlayback::deviceChanged, this, &MainWindow::_q_audioDeviceChanged);
    connect(playback, &IAudioPlayback::deviceAdded, this, &MainWindow::_q_audioDeviceAdded);
    connect(playback, &IAudioPlayback::deviceRemoved, this, &MainWindow::_q_audioDeviceRemoved);

    reloadDevices();
    reloadButtonStatus();
    stopButton->setIcon(QIcon(":/res/stop.svg"));

    resize(750, 10);
}

MainWindow::~MainWindow() {
    playback->dispose();
    decoder->close();
    uninitPlugins();
}

void MainWindow::openFile(const QString &filename) {
    setPlaying(false);
    if (decoder->isOpen()) {
        decoder->SetPosition(0);
        reloadSliderStatus();
        decoder->close();
        timeLabel->setText("--:--/--:--");
    }

    if (!decoder->open({{QsMedia::KEY_NAME_FILE_NAME, filename},
                        {QsMedia::KEY_NAME_SAMPLE_RATE, 44100},
                        {QsMedia::KEY_NAME_SAMPLE_FORMAT, QsMedia::AV_SAMPLE_FMT_FLT},
                        {QsMedia::KEY_NAME_CHANNELS, 2}})) {
        QMessageBox::critical(this, qAppName(), "Failed to initialize decoder!");
        return;
    }

    fileLabel->setText(filename);
    this->filename = filename;

    slider->setMinimum(0);
    slider->setMaximum(decoder->Length());

    reloadSliderStatus();
}

void MainWindow::setPlaying(bool playing) {
    if (this->playing == playing) {
        return;
    }

    this->playing = playing;

    if (playing) {
        playback->play();

        notifyTimerId = this->startTimer(20);
    } else {
        playback->stop();

        killTimer(notifyTimerId);
        notifyTimerId = 0;
    }

    reloadButtonStatus();
    reloadSliderStatus();
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (event->timerId() == notifyTimerId) {
        reloadSliderStatus();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    auto e = static_cast<QDragEnterEvent *>(event);
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls()) {
        auto urls = mime->urls();
        QStringList filenames;
        for (auto it = urls.begin(); it != urls.end(); ++it) {
            if (it->isLocalFile()) {
                filenames.append(Sys::removeTailSlashes(it->toLocalFile()));
            }
        }
        bool ok = false;
        if (filenames.size() == 1) {
            QString filename = filenames.front();
            QString suffix = QFileInfo(filename).suffix().toLower();
            if (filters.contains(suffix)) {
                ok = true;
            }
        }
        if (ok) {
            e->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    auto e = static_cast<QDropEvent *>(event);
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls()) {
        auto urls = mime->urls();
        QStringList filenames;
        for (auto it = urls.begin(); it != urls.end(); ++it) {
            if (it->isLocalFile()) {
                filenames.append(Sys::removeTailSlashes(it->toLocalFile()));
            }
        }
        bool ok = false;
        if (filenames.size() == 1) {
            QString filename = filenames.front();
            QString suffix = QFileInfo(filename).suffix().toLower();
            if (filters.contains(suffix)) {
                ok = true;
                openFile(filename);
            }
        }
        if (ok) {
            e->acceptProposedAction();
        }
    }
}

void MainWindow::initPlugins() {
#ifdef Q_OS_LINUX
    decoderLoader.setFileName("audiodecoders/libFFmpegDecoder");
    playbackLoader.setFileName("audioplaybacks/libSDLPlayback");
#else
    decoderLoader.setFileName("audiodecoders/FFmpegDecoder");
    playbackLoader.setFileName("audioplaybacks/SDLPlayback");
#endif

    decoder = qobject_cast<IAudioDecoder *>(decoderLoader.instance());
    playback = qobject_cast<IAudioPlayback *>(playbackLoader.instance());

    if (!decoder || !playback) {
        QMessageBox::critical(
            this, qAppName(),
            QString("Failed to load plugins: %1!").arg(decoderLoader.errorString()));
        goto out;
    }

    if (!playback->setup(IAudioPlayback::PlaybackArguments{1024, 44100, 2, {}})) {
        QMessageBox::critical(
            this, qAppName(),
            QString("Failed to load playback: %1!").arg(playbackLoader.errorString()));
        goto out;
    }
    playback->setDecoder(decoder);
    return;

out:
    uninitPlugins();
    ::exit(-1);
}

void MainWindow::initStyleSheet() {
    QFile file(":/res/app.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(file.readAll());
    }
}

void MainWindow::uninitPlugins() {
    delete playback;
    delete decoder;
    decoderLoader.unload();
    playbackLoader.unload();
}

void MainWindow::reloadDevices() {
    deviceMenu->clear();

    QStringList devices = playback->devices();
    for (const QString &dev : qAsConst(devices)) {
        auto action = new QAction(dev, deviceMenu);
        action->setCheckable(true);
        action->setData(dev);
        deviceMenu->addAction(action);
        deviceActionGroup->addAction(action);
    }

    reloadDeviceActionStatus();
}

void MainWindow::reloadButtonStatus() {
    playButton->setIcon(QIcon(!playing ? ":/res/play.svg" : ":/res/pause.svg"));
}

void MainWindow::reloadSliderStatus() {
    qint64 max = decoder->Length();
    qint64 pos = decoder->Position();

    if (!slider->isSliderDown()) {
        slider->setValue(pos);
    }

    auto fmt = decoder->outFormat();
    int len_msecs = (double(max) / fmt.SampleRate() / 4 / fmt.Channels()) * 1000;
    int pos_msecs = (double(pos) / fmt.SampleRate() / 4 / fmt.Channels()) * 1000;

    QTime time(0, 0, 0);
    timeLabel->setText(time.addMSecs(pos_msecs).toString("mm:ss") + "/" +
                       time.addMSecs(len_msecs).toString("mm:ss"));
}

void MainWindow::reloadDeviceActionStatus() {
    QString dev = playback->currentDevice();
    const auto &actions = deviceMenu->actions();

    for (QAction *action : actions) {
        action->setChecked(false);
    }
    for (QAction *action : actions) {
        if (action->data().toString() == dev) {
            action->setChecked(true);
            break;
        }
    }
}

void MainWindow::_q_browseActionTriggered() {
    setPlaying(false);

    QString dir = QFileInfo(filename).absolutePath();
    QString path = QFileDialog::getOpenFileName(
        this, "Open Audio", QFileInfo(dir).isDir() ? dir : ".",
        QString("Audio Files(%1)")
            .arg(Math::batchReplace<QString>(
                     filters.values(), //
                     [](const QString &str) -> QString { return "*." + str; })
                     .join(" ")));
    if (path.isEmpty()) {
        return;
    }
    openFile(path);
}

void MainWindow::_q_playButtonClicked() {
    if (!decoder->isOpen()) {
        return;
    }
    setPlaying(!playing);
}

void MainWindow::_q_stopButtonClicked() {
    if (!decoder->isOpen()) {
        return;
    }
    decoder->SetPosition(0);
    setPlaying(false);
}

void MainWindow::_q_sliderReleased() {
    if (!decoder->isOpen()) {
        slider->setValue(0);
        __debugbreak();
        return;
    }

    double percentage = double(slider->value()) / slider->maximum();

    decoder->SetPosition(decoder->Length() * percentage);
    setPlaying(true);
}

void MainWindow::_q_deviceActionTriggered(QAction *action) {
    if (playing) {
        QMessageBox::warning(this, qAppName(), "Stop sound first!");
    } else {
        QString dev = action->data().toString();
        playback->setDevice(dev);
    }

    reloadDeviceActionStatus();
}

void MainWindow::_q_playStateChanged() {
    bool isPlaying = playback->isPlaying();
    if (playing != isPlaying) {
        if (decoder->Position() == decoder->Length()) {
            // Sound complete
            decoder->SetPosition(0);
        }
        setPlaying(isPlaying);
    }
}

void MainWindow::_q_audioDeviceChanged() {
    reloadDeviceActionStatus();
}

void MainWindow::_q_audioDeviceAdded() {
    reloadDevices();
}

void MainWindow::_q_audioDeviceRemoved() {
    reloadDevices();
}
