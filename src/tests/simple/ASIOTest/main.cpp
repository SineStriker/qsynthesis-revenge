#include <AsioSimpleDevice.h>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QFormLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>

static float audio[65536];
static long pos = 0;


static void sineWave(long size, float *buf, double sampleRate) {
    for(long i = 0; i < size; i++) {
        buf[i] = audio[pos];
        pos++;
        pos %= 65536;
    }
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QFile audioFile;
    audioFile.setFileName(":/test.pcm");
    audioFile.open(QFile::ReadOnly);
    audioFile.read(reinterpret_cast<char *>(audio), 65536 * 4);

    AsioSimpleDevice dev;

    QMainWindow win;
    auto mainWidget = new QWidget;
    auto formLayout = new QFormLayout;
    mainWidget->setLayout(formLayout);
    win.setCentralWidget(mainWidget);

    auto drvSelect = new QComboBox;
    drvSelect->addItems(dev.drivers());
    formLayout->addRow("Driver", drvSelect);

    auto specLabel = new QLabel;
    formLayout->addRow(specLabel);

    auto srEditLayout = new QHBoxLayout;
    auto setSampleRateEdit = new QDoubleSpinBox;
    setSampleRateEdit->setRange(0, 192000);
    auto commitSetSampleRateBtn = new QPushButton("Set");
    srEditLayout->addWidget(setSampleRateEdit);
    srEditLayout->addWidget(commitSetSampleRateBtn);
    formLayout->addRow("sr", srEditLayout);

    QObject::connect(&dev, &AsioSimpleDevice::sampleRateChanged, setSampleRateEdit, &QDoubleSpinBox::setValue);
    QObject::connect(commitSetSampleRateBtn, &QPushButton::clicked, [&](){
        if(!dev.setSampleRate(setSampleRateEdit->value())) {
            QMessageBox::critical(&win, "ASIOTest", "Driver does not support this sample rate.");
            setSampleRateEdit->setValue(dev.spec().sampleRate);
        }
    });

    auto stateBtn = new QPushButton;
    auto playbackBtn = new QPushButton;
    playbackBtn->setDisabled(true);

    stateBtn->setText("Initialize");
    QObject::connect(&dev, &AsioSimpleDevice::initializationStatusChanged, stateBtn, [&](bool s){
        stateBtn->setText(s ? "Finalize" : "Initialize");
        drvSelect->setDisabled(s);
        playbackBtn->setDisabled(!s);
        if(s) {
            specLabel->setText(QString("i: %1, o: %2").arg(dev.spec().inputChannels).arg(dev.spec().outputChannels));
            setSampleRateEdit->setDisabled(false);
            commitSetSampleRateBtn->setDisabled(false);
        } else {
            specLabel->setText("");
            setSampleRateEdit->setDisabled(true);
            commitSetSampleRateBtn->setDisabled(true);
        }
    });

    QObject::connect(stateBtn, &QPushButton::clicked, [&](){
        if(dev.isInitialized()) dev.finalize();
        else {
            qDebug() << drvSelect->currentText();
            dev.setName(drvSelect->currentText());
            if(!dev.initialize()) {
                QMessageBox::critical(&win, "ASIOTest", "Cannot initialize ASIO driver");
            }
        }
    });
    formLayout->addRow(stateBtn);

    auto callback = [&](long size, const QVector<float *> &bufferList){
        sineWave(size, bufferList[0], dev.spec().sampleRate);
        for(int i = 1; i < bufferList.size(); i++) {
            memcpy(bufferList[i], bufferList[0], size * 4);
        }
    };
    playbackBtn->setText("Start");
    QObject::connect(&dev, &AsioSimpleDevice::playbackStatusChanged, playbackBtn, [=](bool s){
        playbackBtn->setText(s ? "Stop" : "Start");
    });
    QObject::connect(playbackBtn, &QPushButton::clicked, [&](){
        if(dev.isStarted()) dev.stop();
        else {
            if(!dev.start(callback)) {
                QMessageBox::critical(&win, "ASIOTest", "Cannot start playback");
            }
        }
    });
    formLayout->addRow(playbackBtn);

    win.show();

    app.exec();
}