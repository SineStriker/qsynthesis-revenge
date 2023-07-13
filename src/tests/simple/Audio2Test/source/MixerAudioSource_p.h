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

static inline void applyGainAndPan(const AudioSourceReadData &readData, float gain, float pan) {
    std::array<float, 2> a = {gain * std::max(1.0f, 1.0f - pan), gain * std::max(1.0f, 1.0f + pan)};
    int chCnt = std::min(readData.buffer->channelCount(), 2);
    for(int ch = 0; ch < chCnt; ch++) {
        readData.buffer->gainSampleRange(ch, readData.startPos, readData.length, a[ch]);
    }
}

struct IMixer {
    QMChronMap<AudioSource *, bool> sourceDict;
    QMutex mutex;

    float gain = 1;
    float pan = 0;

    void deleteOwnedSources() const;
    bool start(qint64 bufferSize, double sampleRate) const;
    void stop() const;
};

class MixerAudioSourcePrivate: public AudioSourcePrivate, public IMixer {
    Q_DECLARE_PUBLIC(MixerAudioSource)
public:

};

#endif // CHORUSKIT_MIXERAUDIOSOURCE_P_H
