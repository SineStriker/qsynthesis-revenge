#include "buffer/AudioBuffer.h"
#include "source/PositionableMixerAudioSource.h"
#include "source/SineWaveAudioSource.h"
#include <QDebug>
#include <QFile>

static QString fileName = "C:/Users/Crs_1/test.pcm";
static int length = 1048576, bufferSize = 1024;
static double sampleRate = 48000;

int main(){
    QFile audioFile(fileName);
    AudioBuffer buf(1, bufferSize);

    SineWaveAudioSource src1;
    src1.setFrequency(440);

    SineWaveAudioSource src2;
    src2.setFrequency(440 * sqrt(2));

    PositionableMixerAudioSource src;
    src.addSource(&src1);
    src.addSource(&src2);

    src.open(bufferSize, sampleRate);

    audioFile.open(QFile::WriteOnly);

    qDebug() << src.nextReadPosition();
    int readSize;
    while(src.nextReadPosition() < length && (readSize = src.read(&buf))) audioFile.write((char *)buf.constData(0), readSize * sizeof(float));
    qDebug() << src.nextReadPosition();
    src.setNextReadPosition(length / 2);
    qDebug() << src.nextReadPosition();
    while(src.nextReadPosition() < length && (readSize = src.read(&buf))) audioFile.write((char *)buf.constData(0), readSize * sizeof(float));
    qDebug() << src.nextReadPosition();

    src.close();

    return 0;
}