#include "AudioTrack.h"
#include "Pcm32BitFloatAudioSource.h"
#include <QApplication>
#include <QDebug>
#include <QWindow>
#include <QThread>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
}
extern "C" Q_DECL_EXPORT bool Initializer();
extern "C" Q_DECL_EXPORT void WindowOpener();
extern "C" Q_DECL_EXPORT bool Terminator();
int main(int argc, char *argv[]) {
    Initializer();
    QThread::msleep(1000);
    WindowOpener();
    QThread::msleep(1000);
    Terminator();
    QThread::msleep(1000);
    Initializer();
    QThread::msleep(1000);
    WindowOpener();
    QThread::msleep(1000);
    Terminator();
    QThread::msleep(1000);
    return 0;
}