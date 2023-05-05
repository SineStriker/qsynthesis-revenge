//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_AUDIOTRACK_H
#define CHORUSKIT_AUDIOTRACK_H

#include "AudioBus.h"
#include "IAudioSource.h"

class AudioTrack: public AudioBus, public IAudioSource {
public:
    AudioTrack(quint16 channelCount, quint32 sampleRate);
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
    bool addSource(IAudioSource *src) override;

private:
    quint64 _read(AudioBuffer *buf, quint64 size, ReadMode mode, bool isPeeking);
    quint64 m_pos = 0;
    quint16 m_channelCount;
};



#endif // CHORUSKIT_AUDIOTRACK_H
