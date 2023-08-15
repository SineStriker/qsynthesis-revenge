#include "buffer/AudioBuffer.h"
#include "buffer/InterleavedAudioDataWrapper.h"
#include "device/AudioDevice.h"
#include "device/AudioDriverManager.h"
#include "device/AudioSourcePlayback.h"
#include "device/SDLAudioDriver.h"
#include "source/MemoryAudioSource.h"
#include "source/PositionableMixerAudioSource.h"
#include "source/SineWaveAudioSource.h"
#include "source/TransportAudioSource.h"
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <cmath>

#include "LevelMeter.h"
#include "SeekBar.h"
#include "buffer/AudioClipSeries.h"
#include "format/AudioFormatIO.h"
#include "format/TransportAudioSourceWriter.h"
#include "sndfile.h"
#include "source/AudioFormatInputSource.h"
#include "source/AudioSourceClipSeries.h"

#include <QProgressBar>
#include <sndfile.h>

int main(int argc, char **argv){

    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei UI", 9));

    QMainWindow mainWindow;

    auto mainWidget = new QWidget;

    auto layout = new QFormLayout;

    auto driverComboBox = new QComboBox;

    auto deviceComboBox = new QComboBox;

    auto deviceSpecLabel = new QLabel;

    auto bufferSizeComboBox = new QComboBox;

    auto sampleRateComboBox = new QComboBox;

    auto fileNameLabel = new QLabel;

//    auto transportSlider = new QSlider;
//    transportSlider->setOrientation(Qt::Horizontal);
    auto transportSlider = new SeekBar;
    transportSlider->setMin(0);
    transportSlider->setMax(100);

    auto playPauseButton = new QPushButton("Pause");
    playPauseButton->setDisabled(true);

    auto enableLoopingCheckBox = new QCheckBox("Looping");

    auto loopingStartSlider = new QSlider;
    loopingStartSlider->setOrientation(Qt::Horizontal);
    loopingStartSlider->setDisabled(true);

    auto loopingEndSlider = new QSlider;
    loopingEndSlider->setOrientation(Qt::Horizontal);
    loopingEndSlider->setDisabled(true);

    auto fileSpecLabel = new QLabel;

//    auto leftLevelMeter = new QProgressBar;
//    leftLevelMeter->setRange(0, 60);
//    leftLevelMeter->setTextVisible(false);
    auto leftLevelMeter = new LevelMeter(Qt::Horizontal, LevelMeter::Stereo);
    leftLevelMeter->initBuffer(8);

//    auto rightLevelMeter = new QProgressBar;
//    rightLevelMeter->setRange(0, 60);
//    rightLevelMeter->setTextVisible(false);
    auto rightLevelMeter = new LevelMeter(Qt::Vertical, LevelMeter::Stereo);
    rightLevelMeter->initBuffer(8);

        auto browseFileButton = new QPushButton("Browse");

    auto startButton = new QPushButton("Start");

    auto resetPosButton = new QPushButton("Reset Position");

    auto stopButton = new QPushButton("Stop");
    stopButton->setDisabled(true);

    auto exportButton = new QPushButton("Export Audio");

    layout->addRow("Driver", driverComboBox);
    layout->addRow("Device", deviceComboBox);
    layout->addRow(deviceSpecLabel);
    layout->addRow("Buffer Size", bufferSizeComboBox);
    layout->addRow("Sample Rate", sampleRateComboBox);
    layout->addRow(fileNameLabel);
    layout->addRow(fileSpecLabel);
    layout->addRow(browseFileButton);
    layout->addRow("Level (L)", leftLevelMeter);
    layout->addRow("Level (R)", rightLevelMeter);
    layout->addRow("Transport", transportSlider);
    layout->addRow(playPauseButton);
    layout->addRow(enableLoopingCheckBox);
    layout->addRow("Loop Start", loopingStartSlider);
    layout->addRow("Loop End", loopingEndSlider);
    layout->addRow(startButton);
    layout->addRow(resetPosButton);
    layout->addRow(stopButton);
    layout->addRow(exportButton);

    mainWidget->setLayout(layout);
    mainWindow.setCentralWidget(mainWidget);

    AudioDriverManager drvMgr;

    for(auto drv: SDLAudioDriver::getDrivers()) {
        drvMgr.addAudioDriver(drv);
    }

    driverComboBox->addItems(drvMgr.drivers());

    AudioDriver *driver = nullptr;
    AudioDevice *device = nullptr;

    QObject *deviceComboBoxCtx = nullptr;
    QObject *driverComboBoxCtx = nullptr;

    QList<QFile *> srcFileList;
    QList<AudioFormatIO *> srcIoList;
    QList<AudioFormatInputSource *> srcList;
    QList<PositionableMixerAudioSource *> trackSrcList;
    PositionableMixerAudioSource mixer;

    TransportAudioSource transportSrc;
    transportSrc.setSource(&mixer);
    AudioSourcePlayback playback(&transportSrc);

    qint64 effectiveLength = 0;

    auto reloadFile = [&](const QString &fileName) {
        if(fileName.isEmpty()) return;
        device->lock();
        mixer.removeAllSource();
        for(auto ptr: trackSrcList) delete ptr;
        for(auto ptr: srcIoList) delete ptr;
        for(auto ptr: srcFileList) delete ptr;
        srcFileList.clear();
        srcIoList.clear();
        trackSrcList.clear();

        fileNameLabel->setText(fileName);
        QFile f(fileName);
        f.open(QFile::ReadOnly);
        auto doc = QJsonDocument::fromJson(f.readAll());
        for(const auto &audioFileNameJsonVal: doc.object().value("audioFiles").toArray()) {
            auto audioFile = new QFile(audioFileNameJsonVal.toString());
            qDebug() << audioFileNameJsonVal.toString();
            srcFileList.append(audioFile);
            srcIoList.append(new AudioFormatIO(audioFile));
        }
        for(const auto &trackSpec: doc.object().value("tracks").toArray()) {
            auto clipSeries = new AudioSourceClipSeries;
            auto trackSrc = new PositionableMixerAudioSource;
            trackSrc->addSource(clipSeries, true);
            trackSrcList.append(trackSrc);
            trackSrc->setGain(trackSpec.toObject().value("gain").toDouble(1.0));
            trackSrc->setPan(trackSpec.toObject().value("pan").toDouble(0.0));
            for(const auto &clipSpec: trackSpec.toObject().value("clips").toArray()) {
                auto audioId = clipSpec.toObject().value("audio").toInt();
                auto positionSec = clipSpec.toObject().value("pos").toDouble();
                auto startPosSec = clipSpec.toObject().value("start").toDouble();
                auto lengthSec = clipSpec.toObject().value("length").toDouble();
                qint64 position = positionSec * device->sampleRate();
                qint64 startPos = startPosSec * device->sampleRate();
                qint64 length = lengthSec * device->sampleRate();
                auto src = new AudioFormatInputSource(srcIoList[audioId]);
                srcList.append(src);
                if(!clipSeries->addClip({position, src, startPos, length})) {
                    QMessageBox::critical(&mainWindow, "Mixer", "Cannot add clip.");
                }
            }
            effectiveLength = std::max(effectiveLength, clipSeries->effectiveLength());
            mixer.addSource(trackSrc);
        }
        device->unlock();
        qint64 audioLength = effectiveLength;
        transportSlider->setRange(0, audioLength - 1);
        loopingStartSlider->setRange(0, audioLength - 1);
        loopingEndSlider->setRange(0, audioLength);
        loopingEndSlider->setValue(audioLength);
    };

    auto restartDevice = [&](){
        if(!device) return;
        device->close();
        if(!device->open(bufferSizeComboBox->currentText().toULongLong(), sampleRateComboBox->currentText().toDouble())) {
            QMessageBox::critical(&mainWindow, "Device Error", device->errorString());
        }
        reloadFile(fileNameLabel->text());
    };

    QObject::connect(driverComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index){
        if(device) {
            device->close();
        }
        if(driver) {
            driver->finalize();
        }
        delete driverComboBoxCtx;
        if(driverComboBox->itemText(index).isEmpty()) return;
        driver = drvMgr.driver(driverComboBox->itemText(index));
        deviceComboBox->clear();
        if(!driver->initialize()) {
            QMessageBox::critical(&mainWindow, "Driver Error", driver->errorString());
            return;
        }
        auto defaultDev = driver->defaultDevice();
        auto deviceList = driver->devices();
        auto defaultDevIndex = deviceList.indexOf(defaultDev);
        deviceComboBox->addItems(deviceList);
        if(defaultDevIndex != -1) deviceComboBox->setCurrentIndex(defaultDevIndex);
        driverComboBoxCtx = new QObject;
        QObject::connect(deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), driverComboBoxCtx, [&](int index){
            if(device) {
                device->close();
            }
            delete deviceComboBoxCtx;
            if(deviceComboBox->itemText(index).isEmpty()) return;
            if(!driver) device = nullptr;
            device = driver->createDevice(deviceComboBox->itemText(index));
            deviceSpecLabel->setText(QString("preferred buffer size: %1, preferred sample rate: %2").arg(device->preferredBufferSize()).arg(device->preferredSampleRate()));
            auto availableBufferSizes = device->availableBufferSizes();
            bufferSizeComboBox->clear();
            for(int i = 0; i < availableBufferSizes.size(); i++) {
                bufferSizeComboBox->addItem(QString::number(availableBufferSizes[i]));
                if(availableBufferSizes[i] == device->preferredBufferSize()) bufferSizeComboBox->setCurrentIndex(i);
            }
            auto availableSampleRates = device->availableSampleRates();
            sampleRateComboBox->clear();
            for(int i = 0; i < availableSampleRates.size(); i++) {
                sampleRateComboBox->addItem(QString::number(availableSampleRates[i]));
                if(availableSampleRates[i] == device->preferredSampleRate()) sampleRateComboBox->setCurrentIndex(i);
            }
            deviceComboBoxCtx = new QObject;
            QObject::connect(bufferSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), deviceComboBoxCtx, restartDevice);
            QObject::connect(sampleRateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), deviceComboBoxCtx, restartDevice);
            restartDevice();
        });
        if(deviceComboBox->count()) emit deviceComboBox->currentIndexChanged(deviceComboBox->currentIndex());
    });

    if(driverComboBox->count()) emit driverComboBox->currentIndexChanged(driverComboBox->currentIndex());

    auto availableFormats = AudioFormatIO::availableFormats();

    QObject::connect(&mixer, &PositionableMixerAudioSource::meterUpdated, fileSpecLabel, [=](float ml, float mr){
        QString text = "bm: ";
        if(ml == 0) {
            text += "-inf dB, ";
//            leftLevelMeter->reset();
//            leftLevelMeter->readSample(0);
        } else {
            double db = 10 * log(ml/1.0) / log(10);
            text += QString::number(db, 'f', 1) + " dB, ";
//            leftLevelMeter->setValue(60 + db);
//            leftLevelMeter->readSample(ml);
        }
        if(mr == 0) {
            text += "-inf dB, ";
//            rightLevelMeter->reset();
//            rightLevelMeter->readSample(0);
        } else {
            double db = 10 * log(mr/1.0) / log(10);
            text += QString::number(db, 'f', 1) + " dB, ";
//            rightLevelMeter->setValue(60 + db);
//            rightLevelMeter->readSample(mr);
        }
        leftLevelMeter->readSample(ml, mr);
        rightLevelMeter->readSample(ml, mr);
        fileSpecLabel->setText(text);
    });

    QObject::connect(browseFileButton, &QPushButton::clicked, [&](){
        auto fileName = QFileDialog::getOpenFileName(&mainWindow, {}, {}, "*.json");
        reloadFile(fileName);
    });

    QObject::connect(startButton, &QPushButton::pressed, [&](){
        if(!device) return;
        if(!device->start(&playback)) {
            QMessageBox::critical(&mainWindow, "Playback Error", device->errorString());
        }
        deviceComboBox->setDisabled(true);
        driverComboBox->setDisabled(true);
        bufferSizeComboBox->setDisabled(true);
        sampleRateComboBox->setDisabled(true);
        playPauseButton->setDisabled(false);
        playPauseButton->setText("Pause");
        startButton->setDisabled(true);
        stopButton->setDisabled(false);
        transportSrc.play();
    });

    QObject::connect(resetPosButton, &QPushButton::clicked, [&](){
        transportSrc.setPosition(0);
    });

    QObject::connect(&transportSrc, &TransportAudioSource::positionAboutToChange, transportSlider, [&](qint64 value){
        QSignalBlocker blocker(transportSlider);
        transportSlider->setValue(value);
    });
//    QObject::connect(transportSlider, &QSlider::valueChanged, &transportSrc, &TransportAudioSource::setPosition);
    QObject::connect(transportSlider, &SeekBar::valueChanged, &transportSrc, &TransportAudioSource::setPosition);

    QObject::connect(playPauseButton, &QPushButton::clicked, [&](){
        if(transportSrc.isPlaying()) {
            transportSrc.pause();
            playPauseButton->setText("Play");
        } else {
            transportSrc.play();
            playPauseButton->setText("Pause");
        }
    });

    auto updateLoopingRange = [&](){
        if(enableLoopingCheckBox->isChecked()) {
            transportSrc.setLoopingRange(loopingStartSlider->value(), loopingEndSlider->value());
        } else {
            transportSrc.setLoopingRange(-1, -1);
        }
    };

    QObject::connect(enableLoopingCheckBox, &QCheckBox::clicked, [&](bool checked){
        loopingStartSlider->setDisabled(!checked);
        loopingEndSlider->setDisabled(!checked);
    });

    QObject::connect(enableLoopingCheckBox, &QCheckBox::clicked, updateLoopingRange);

    QObject::connect(loopingStartSlider, &QSlider::valueChanged, updateLoopingRange);

    QObject::connect(loopingEndSlider, &QSlider::valueChanged, updateLoopingRange);

    QObject::connect(stopButton, &QPushButton::clicked, [&](){
        device->stop();
        deviceComboBox->setDisabled(false);
        driverComboBox->setDisabled(false);
        bufferSizeComboBox->setDisabled(false);
        sampleRateComboBox->setDisabled(false);
        playPauseButton->setDisabled(true);
        playPauseButton->setText("Pause");
        startButton->setDisabled(false);
        stopButton->setDisabled(true);
    });

    QObject::connect(exportButton, &QPushButton::clicked, [&](){
        auto exportFileName = QFileDialog::getSaveFileName(&mainWindow, {}, {}, "*.wav");
        if(exportFileName.isEmpty()) return;

        emit stopButton->clicked();

        auto curBufSize = transportSrc.bufferSize();
        auto curSampleRate = device->sampleRate();
        auto curPos = transportSrc.position();

        transportSrc.close();

        QFile exportFile(exportFileName);
        AudioFormatIO exportIO(&exportFile);
        exportIO.open(QFile::WriteOnly, AudioFormatIO::WAV | AudioFormatIO::PCM_24, 2, curSampleRate);

        transportSrc.open(65536, curSampleRate);
        transportSrc.setPosition(0);
        QDialog dlg;
        dlg.setWindowTitle("Exporting...");
        auto dlgLayout = new QVBoxLayout;
        auto exportProgressBar = new QProgressBar;
        dlgLayout->addWidget(exportProgressBar);
        dlg.setLayout(dlgLayout);
        QThread thread;
        TransportAudioSourceWriter writer(&transportSrc, &exportIO, 0, effectiveLength);
        writer.moveToThread(&thread);
        QObject::connect(&thread, &QThread::started, &writer, &TransportAudioSourceWriter::start);
        QObject::connect(&writer, &TransportAudioSourceWriter::percentageUpdated, exportProgressBar, &QProgressBar::setValue);
        QObject::connect(&writer, &TransportAudioSourceWriter::completed, &dlg, &QDialog::accept);
        QObject::connect(&dlg, &QDialog::rejected, &thread, [&](){
            writer.interrupt();
        });
        QObject::connect(&writer, &TransportAudioSourceWriter::interrupted, &mainWindow, [&]{
            QMessageBox::warning(&mainWindow, "Export", "Exporting is interrupted.");
        });
        thread.start();
        dlg.exec();
        transportSrc.close();
        transportSrc.setPosition(curPos);
//        leftLevelMeter->reset();
//        rightLevelMeter->reset();
        leftLevelMeter->readSample(0, 0);
        rightLevelMeter->readSample(0, 0);
        thread.quit();
        thread.wait();
    });

    mainWindow.show();

    return a.exec();
}