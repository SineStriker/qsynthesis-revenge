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

#include "sndfile.h"

int main(int argc, char **argv){

    QApplication a(argc, argv);

    QMainWindow mainWindow;

    auto mainWidget = new QWidget;

    auto layout = new QFormLayout;

    auto driverComboBox = new QComboBox;

    auto deviceComboBox = new QComboBox;

    auto deviceSpecLabel = new QLabel;

    auto bufferSizeComboBox = new QComboBox;

    auto sampleRateComboBox = new QComboBox;

    auto fileNameLabel = new QLabel;

    auto transportSlider = new QSlider;
    transportSlider->setOrientation(Qt::Horizontal);

    auto playPauseButton = new QPushButton("Pause");
    playPauseButton->setDisabled(true);

    auto enableLoopingCheckBox = new QCheckBox("Looping");

    auto loopingStartSlider = new QSlider;
    loopingStartSlider->setOrientation(Qt::Horizontal);
    loopingStartSlider->setDisabled(true);

    auto loopingEndSlider = new QSlider;
    loopingEndSlider->setOrientation(Qt::Horizontal);
    loopingEndSlider->setDisabled(true);

    auto browseFileButton = new QPushButton("Browse (Raw data, 2ch, f32)");

    auto startButton = new QPushButton("Start");

    auto resetPosButton = new QPushButton("Reset Position");

    auto stopButton = new QPushButton("Stop");
    stopButton->setDisabled(true);

    layout->addRow("Driver", driverComboBox);
    layout->addRow("Device", deviceComboBox);
    layout->addRow(deviceSpecLabel);
    layout->addRow("Buffer Size", bufferSizeComboBox);
    layout->addRow("Sample Rate", sampleRateComboBox);
    layout->addRow(fileNameLabel);
    layout->addRow(browseFileButton);
    layout->addRow("Transport", transportSlider);
    layout->addRow(playPauseButton);
    layout->addRow(enableLoopingCheckBox);
    layout->addRow("Loop Start", loopingStartSlider);
    layout->addRow("Loop End", loopingEndSlider);
    layout->addRow(startButton);
    layout->addRow(resetPosButton);
    layout->addRow(stopButton);

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

    auto restartDevice = [&](){
        if(!device) return;
        device->close();
        if(!device->open(bufferSizeComboBox->currentText().toULongLong(), sampleRateComboBox->currentText().toDouble())) {
            QMessageBox::critical(&mainWindow, "Device Error", device->error());
        }
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
            QMessageBox::critical(&mainWindow, "Driver Error", driver->error());
            return;
        }
        auto defaultDev = driver->defaultDevice();
        auto deviceList = driver->devices();
        auto defaultDevIndex = deviceList.indexOf(defaultDev);
        deviceComboBox->addItems(deviceList);
        if(defaultDevIndex != -1) deviceComboBox->setCurrentIndex(defaultDevIndex);
        driverComboBoxCtx = new QObject;
        QObject::connect(driver, &AudioDriver::deviceChanged, driverComboBoxCtx, [&](){
            QMessageBox::information(&mainWindow, "Device changed", "Audio device is changed.");
            emit driverComboBox->currentIndexChanged(driverComboBox->currentIndex());
        });
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

    QByteArray audioFileData;
    InterleavedAudioDataWrapper audioFileBuffer(nullptr, 2, 0);
    MemoryAudioSource src(&audioFileBuffer);
    TransportAudioSource transportSrc;
    transportSrc.resetSource(&src);
    AudioSourcePlayback playback(&transportSrc);

    QObject::connect(browseFileButton, &QPushButton::clicked, [&](){
        auto fileName = QFileDialog::getOpenFileName(&mainWindow);
        if(fileName.isEmpty()) return;
        fileNameLabel->setText(fileName);
        QFile f(fileName);
        f.open(QFile::ReadOnly);
        audioFileData = f.readAll();
        qint64 audioLength = audioFileData.size() / 8;
        transportSlider->setRange(0, audioLength - 1);
        loopingStartSlider->setRange(0, audioLength - 1);
        loopingEndSlider->setRange(0, audioLength);
        loopingEndSlider->setValue(audioLength);
        audioFileBuffer.resetData((float *)audioFileData.data(), 2, audioLength);
    });

    QObject::connect(startButton, &QPushButton::clicked, [&](){
        if(!device) return;
        if(!device->start(&playback)) {
            QMessageBox::critical(&mainWindow, "Playback Error", device->error());
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
    QObject::connect(transportSlider, &QSlider::valueChanged, &transportSrc, &TransportAudioSource::setPosition);

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

    mainWindow.show();

    return a.exec();
}