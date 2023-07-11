#include "buffer/AudioBuffer.h"
#include "buffer/InterleavedAudioDataWrapper.h"
#include "device/AudioDevice.h"
#include "device/AudioSourcePlayback.h"
#include "device/SDLAudioDriver.h"
#include "source/MemoryAudioSource.h"
#include "source/PositionableMixerAudioSource.h"
#include "source/SineWaveAudioSource.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>

static QString fileName = "D:/Downloads/test.pcm";
static int length = 1048576, bufferSize = 1024;
static double sampleRate = 48000;

int main(int argc, char **argv){

    QCoreApplication a(argc, argv);

    QFile audioFile(fileName);
    audioFile.open(QFile::ReadOnly);
    QByteArray audioData = audioFile.readAll();
    auto audioBuf = (float*)audioData.data();
    InterleavedAudioDataWrapper testAudio(audioBuf, 2, audioData.size() / 8);
    MemoryAudioSource src3(&testAudio);

    AudioBuffer buf(1, bufferSize);

    SineWaveAudioSource src1;
    src1.setFrequency(440);

    SineWaveAudioSource src2;
    src2.setFrequency(440 * sqrt(2));

    PositionableMixerAudioSource src;
//    src.addSource(&src1);
//    src.addSource(&src2);
    src.addSource(&src3);

    auto driver = SDLAudioDriver::getDrivers()[0];

    driver->initialize();

    auto dev = driver->createDevice(driver->defaultDevice());

    dev->setActiveChannelCount(1);

    qDebug() << "SDL device:" << dev->name() << dev->channelCount() << dev->preferredBufferSize() << dev->preferredSampleRate();

    if(!dev->open(dev->preferredBufferSize(), dev->preferredSampleRate())) {
        qDebug() << dev->error();
    }

    qDebug() << "SDL device:" << dev->activeChannelCount();

    if(!dev->start(new AudioSourcePlayback(&src))) {
        qDebug() << dev->error();
    }

    return a.exec();
}