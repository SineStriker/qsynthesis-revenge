#include <AsioSimpleDevice.h>
#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QLabel>

static void sineWave(long size, float *buf, double sampleRate) {
    for(long i = 0; i < size; i++) {
        buf[i] = sin(2 * 3.141592653589793 * 440 / sampleRate * i);
    }
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

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

    auto stateBtn = new QPushButton;
    auto playbackBtn = new QPushButton;
    playbackBtn->setDisabled(true);

    stateBtn->setText("Initialize");
    QObject::connect(&dev, &AsioSimpleDevice::initializationStatusChanged, stateBtn, [&](bool s){
        stateBtn->setText(s ? "Finalize" : "Initialize");
        drvSelect->setDisabled(s);
        playbackBtn->setDisabled(!s);
        if(s) {
            specLabel->setText(QString("i: %1, o: %2, sr: %3").arg(dev.spec().inputChannels).arg(dev.spec().outputChannels).arg(dev.spec().sampleRate));
        } else {
            specLabel->setText("");
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

    auto callback = [&](int i, long size, float *buf){
        sineWave(size, buf, dev.spec().sampleRate);
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