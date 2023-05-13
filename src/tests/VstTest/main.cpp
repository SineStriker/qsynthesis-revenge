//
// Created by Crs_1 on 2023/5/13.
//
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>

#include "IpcServer.h"
#include "IpcSendChannel.h"

using namespace Vst;

extern "C" Q_DECL_EXPORT void Initializer();
extern "C" Q_DECL_EXPORT void Terminator();
extern "C" Q_DECL_EXPORT void ProcessInitializer(bool isOffline, qint32 maxBufferSize, double sampleRate);
struct PlaybackParameters {
    double sampleRate;
    qint64 position;
    qint32 bufferSize;
};
extern "C" Q_DECL_EXPORT bool PlaybackProcessor(const PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const *const *outputs);

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    auto *ipcServer = new IpcServer("77F6E993-671E-4283-99BE-C1CD1FF5C09E", nullptr);
    auto *ipcSendChannel = new IpcSendChannel(ipcServer);
    auto *w = new QMainWindow;
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout;
    auto *lineEdit = new QLineEdit;
    auto *btn = new QPushButton;
    float **p1[16];
    float *p2[2 * 16];
    float buf[4096 * 2 * 16];
    QObject::connect(btn, &QPushButton::clicked, [&](){
        for(int i = 0; i < 16; i++)p1[i] = &p2[2 * i];
        for(int i = 0; i < 2 * 16; i++)p2[i] = &buf[4096 * i];
        PlaybackParameters param = {48000, 0, 4096};
        PlaybackProcessor(&param, false, 1, p1);
        qDebug() << buf[20] << buf[40];
    });
    layout->addWidget(lineEdit);
    layout->addWidget(btn);
    widget->setLayout(layout);
    w->setCentralWidget(widget);
    w->show();
    Initializer();
    ProcessInitializer(false, 4096, 48000);
    a.exec();
}
