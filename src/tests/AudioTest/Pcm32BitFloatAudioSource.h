//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_PCM32BITFLOATAUDIOSOURCE_H
#define CHORUSKIT_PCM32BITFLOATAUDIOSOURCE_H


#include <QFile>
#include "IAudioSource.h"

class Pcm32BitFloatAudioSource : public IAudioSource {
public:
    Pcm32BitFloatAudioSource(const QString &fileName, quint16 channelCount, quint32 sampleRate);
    quint64 read(AudioBufferList &buf, quint64 size) override;
    quint64 peek(AudioBufferList &buf, quint64 size, quint64 offset) override;
    quint64 pos() const override;
    bool setPos(quint64 pos) override;
    bool isSequential() const override;
    quint32 sampleRate() const override;
    bool isSampleRateChangeable() const override;
    quint16 channelCount() const override;
    quint64 readableSampleCount() const override;
    bool open() override;
    void close() override;
private:
    QFile file;
    quint32 m_sampleRate;
    quint16 m_channelCount;
    quint64 _read(AudioBufferList &buf, quint64 size, quint64 offset, bool isPeeking);
};



#endif // CHORUSKIT_PCM32BITFLOATAUDIOSOURCE_H
