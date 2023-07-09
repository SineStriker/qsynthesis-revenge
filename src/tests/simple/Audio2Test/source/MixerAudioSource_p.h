//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_MIXERAUDIOSOURCE_P_H
#define CHORUSKIT_MIXERAUDIOSOURCE_P_H

#include "MixerAudioSource.h"

#include <QMutex>

#include <QMChronMap.h>

#include "AudioSource_p.h"

struct IMixer {
    QMChronMap<AudioSource *, bool> sourceDict;
    QMutex mutex;
    void deleteOwnedSources();
    bool start(int bufferSize, double sampleRate);
    void stop();
};

class MixerAudioSourcePrivate: public AudioSourcePrivate, public IMixer {
    Q_DECLARE_PUBLIC(MixerAudioSource)
public:

};

#endif // CHORUSKIT_MIXERAUDIOSOURCE_P_H
