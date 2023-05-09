//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_AUDIOTRACK_H
#define CHORUSKIT_AUDIOTRACK_H

#include "AudioBus.h"
#include "IAudioSource.h"

class AudioTrack: public QObject, public AudioBus, public IAudioSource {
    Q_OBJECT
public:
    AudioTrack(quint16 channelCount, quint32 sampleRate);
    quint64 read(AudioBufferList &buf, quint64 size) override;
    quint64 peek(AudioBufferList &buf, quint64 size, quint64 offset) override;
    quint64 pos() override;
    bool setPos(quint64 pos) override;
    bool isSequential() override;
    quint32 sampleRate() override;
    bool setSampleRate(quint32 sampleRate) override;
    bool isSampleRateChangeable() override;
    quint16 channelCount() override;
    quint64 readableSampleCount() override;
    bool addSource(IAudioSource *src) override;
    bool open() override;
    void close() override;
signals:
    void audioRead(const AudioBufferList &buf, quint64 size);
private:
    quint64 _read(AudioBufferList &buf, quint64 size, quint64 offset, bool isPeeking);
    quint64 m_pos = 0;
    quint16 m_channelCount;
};



#endif // CHORUSKIT_AUDIOTRACK_H
