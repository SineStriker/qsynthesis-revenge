//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_MIXERAUDIOSOURCE_H
#define CHORUSKIT_MIXERAUDIOSOURCE_H

#include <QList>
#include <QObject>

#include "AudioSource.h"

class MixerAudioSourcePrivate;

class MixerAudioSource: public QObject, public AudioSource {
    Q_OBJECT
#define d_ptr AudioSource::d_ptr
    Q_DECLARE_PRIVATE(MixerAudioSource)
#undef d_ptr
public:

    MixerAudioSource();
    ~MixerAudioSource();

    bool open(qint64 bufferSize, double sampleRate) override;
    qint64 read(const AudioSourceReadData &readData) override;
    void close() override;

    bool addSource(AudioSource *src, bool takeOwnership = false);
    bool removeSource(AudioSource *src);
    void removeAllSource();
    QList<AudioSource *> sources() const;

    void setGain(float gain);
    float gain() const;

    void setPan(float pan);
    float pan() const;

signals:
    void meterUpdated(float leftMagnitude, float rightMagnitude);

protected:
    explicit MixerAudioSource(MixerAudioSourcePrivate &d);
};



#endif // CHORUSKIT_MIXERAUDIOSOURCE_H
