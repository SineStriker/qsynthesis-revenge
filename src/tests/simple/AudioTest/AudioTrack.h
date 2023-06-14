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
    quint64 pos() const override;
    bool setPos(quint64 pos) override;
    bool isSequential() const override;
    quint32 sampleRate() const override;
    bool setSampleRate(quint32 sampleRate) override;
    bool isSampleRateChangeable() const override;
    quint16 channelCount() const override;
    quint64 readableSampleCount() const override;
    bool addSource(IAudioSource *src) override;
    bool open() override;
    void close() override;
    ReadMode readMode() const override;
    bool setReadMode(ReadMode mode) override;
    void setVolume(const QVector<float> &rates);
    void setVolume(float rate);
    QVector<float> volume();
signals:
    void audioRead(const AudioBufferList &buf, quint64 size);
private:
    quint64 _read(AudioBufferList &buf, quint64 size, quint64 offset, bool isPeeking);
    quint64 m_pos = 0;
    quint16 m_channelCount;
    QVector<float> m_volumeRates;
    ReadMode m_readMode = Immediate;
};



#endif // CHORUSKIT_AUDIOTRACK_H
