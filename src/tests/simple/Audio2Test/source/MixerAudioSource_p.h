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

template<class T>
struct IMixer {
    QMChronMap<T *, bool> sourceDict;
    QMutex mutex;

    float gain = 1;
    float pan = 0;
    AudioBuffer tmpBuf;

    void deleteOwnedSources() const {
        for(auto src: sourceDict.keys()) {
            if(sourceDict.value(src)) {
                delete src;
            }
        }
    }
    bool start(qint64 bufferSize, double sampleRate) const {
        auto sourceList = sourceDict.keys();
        if(std::all_of(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
                return src->open(bufferSize, sampleRate);
            })) {
            return true;
        } else {
            return false;
        }
    }
    void stop() const {
        auto sourceList = sourceDict.keys();
        std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){ src->close();
        });
    }
};

class MixerAudioSourcePrivate: public AudioSourcePrivate, public IMixer<AudioSource> {
    Q_DECLARE_PUBLIC(MixerAudioSource)
public:

};

#endif // CHORUSKIT_MIXERAUDIOSOURCE_P_H
