//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_MIXERAUDIOSOURCE_P_H
#define CHORUSKIT_MIXERAUDIOSOURCE_P_H

#include "MixerAudioSource.h"

#include <array>

#include <QMutex>
#include <QMChronMap.h>

#include "AudioSource_p.h"
#include "buffer/AudioBuffer.h"

static inline QPair<float, float> applyGainAndPan(float gain, float pan) {
    return {gain * std::max(1.0f, 1.0f - pan), gain * std::max(1.0f, 1.0f + pan)};
}

struct IMixer {
    QMChronMap<AudioSource *, bool> sourceDict;
    QMutex mutex;

    float gain = 1;
    float pan = 0;
    AudioBuffer tmpBuf;

    void deleteOwnedSources() const;
    bool start(qint64 bufferSize, double sampleRate) const;
    void stop() const;
};

class MixerAudioSourcePrivate: public AudioSourcePrivate, public IMixer {
    Q_DECLARE_PUBLIC(MixerAudioSource)
public:

};

#endif // CHORUSKIT_MIXERAUDIOSOURCE_P_H
