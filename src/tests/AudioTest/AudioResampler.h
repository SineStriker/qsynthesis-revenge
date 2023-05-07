//
// Created by Crs_1 on 2023/5/6.
//

#ifndef CHORUSKIT_AUDIORESAMPLER_H
#define CHORUSKIT_AUDIORESAMPLER_H



#include "AudioBus.h"
class AudioResampler: public IAudioSource {
public:
    AudioResampler(IAudioSource *src);
    quint64 read(AudioBuffer *buf, quint64 size, ReadMode mode) override;
    quint64 peek(AudioBuffer *buf, quint64 size, ReadMode mode) override;
    quint64 pos() override;
    bool setPos(quint64 pos) override;
    quint32 sampleRate() override;
    bool setSampleRate(quint32 sampleRate) override;
    bool isSampleRateChangeable() override;
    quint16 channelCount() override;
    quint64 readableSampleCount() override;
    bool canRead(quint64 size) override;
    bool open() override;
    bool close() override;
private:
    IAudioSource *m_src;
    quint64 m_sampleRate;
    QVector<float> buffer;
};



#endif // CHORUSKIT_AUDIORESAMPLER_H
