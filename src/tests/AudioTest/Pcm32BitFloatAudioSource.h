//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_PCM32BITFLOATAUDIOSOURCE_H
#define CHORUSKIT_PCM32BITFLOATAUDIOSOURCE_H


#include <QFile>
#include "IAudioSource.h"

class Pcm32BitFloatAudioSource : public IAudioSource {
public:
    Pcm32BitFloatAudioSource(const QString &fileName, quint32 sampleRate, quint16 channelCount);
    quint64 read(AudioBuffer *buf, quint64 size, ReadMode mode) override;
    quint64 peek(AudioBuffer *buf, quint64 size, ReadMode mode) override;
    quint64 pos() override;
    bool setPos(quint64 pos) override;
    bool isSequential() override;
    quint32 sampleRate() override;
    bool setSampleRate(quint32 sampleRate) override;
    bool isSampleRateChangeable() override;
    quint16 channelCount() override;
    quint64 readableSampleCount() override;
    bool canRead(quint64 size) override;
private:
    QFile file;
    quint32 m_sampleRate;
    quint16 m_channelCount;
    quint64 _read(AudioBuffer *buf, quint64 size, ReadMode mode, bool isPeeking);
};



#endif // CHORUSKIT_PCM32BITFLOATAUDIOSOURCE_H
