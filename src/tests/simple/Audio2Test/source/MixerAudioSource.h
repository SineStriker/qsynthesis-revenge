//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_MIXERAUDIOSOURCE_H
#define CHORUSKIT_MIXERAUDIOSOURCE_H

#include <QList>
#include <QMap>

#include <QMChronMap.h>

#include "AudioSource.h"

class MixerAudioSource: public AudioSource {

public:

    ~MixerAudioSource();

    bool start(int bufferSize, double sampleRate) override;
    bool isStarted() const override;
    int read(const AudioSourceReadData &readData) override;
    void stop() override;

    void addSource(AudioSource *src, bool takeOwnership = false);
    void removeSource(AudioSource *src);
    void removeAllSource();
    QList<AudioSource *> sources() const;

private:
    QMChronMap<AudioSource *, bool> m_sourceDict;
    bool m_isStarted = false;
};



#endif // CHORUSKIT_MIXERAUDIOSOURCE_H
