//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_MIXERAUDIOSOURCE_H
#define CHORUSKIT_MIXERAUDIOSOURCE_H

#include <QList>

#include "AudioSource.h"

class MixerAudioSourcePrivate;

class MixerAudioSource: public AudioSource {
    Q_DECLARE_PRIVATE(MixerAudioSource)
public:

    MixerAudioSource();
    ~MixerAudioSource();

    bool start(int bufferSize, double sampleRate) override;
    int read(const AudioSourceReadData &readData) override;
    void stop() override;

    void addSource(AudioSource *src, bool takeOwnership = false);
    void removeSource(AudioSource *src);
    void removeAllSource();
    QList<AudioSource *> sources() const;

protected:
    MixerAudioSource(MixerAudioSourcePrivate &d);
};



#endif // CHORUSKIT_MIXERAUDIOSOURCE_H
