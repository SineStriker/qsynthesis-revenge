//
// Created by Crs_1 on 2023/7/9.
//

#include "IAudioSampleProvider.h"
#include "IAudioSampleContainer.h"
#include <algorithm>
#include <cassert>
#include <cmath>

static inline void boundCheck(const IAudioSampleProvider &iAudioStorage, int channel, qint64 startPos, qint64 length) {
    assert(channel >= 0 && channel < iAudioStorage.channelCount());
    assert(startPos >= 0 && startPos <= iAudioStorage.sampleCount());
    assert(startPos + length >= 0 && startPos + length <= iAudioStorage.sampleCount());
}

float IAudioSampleProvider::magnitude(int channel, qint64 startPos, qint64 length) const {
    boundCheck(*this, channel, startPos, length);
    float m = 0;
    for(qint64 i = 0; i < length; i++) {
        m = std::max(m, std::abs(constSampleAt(channel, startPos + i)));
    }
    return m;
}
float IAudioSampleProvider::magnitude(int channel) const {
    return magnitude(channel, 0, sampleCount());
}
float IAudioSampleProvider::rms(int channel, qint64 startPos, qint64 length) const {
    boundCheck(*this, channel, startPos, length);
    float s = 0;
    for(qint64 i = 0; i < length; i++) {
        auto sample = constSampleAt(channel, startPos + i);
        s += sample * sample;
    }
    return sqrt(s / length);
}
float IAudioSampleProvider::rms(int channel) const {
    return rms(channel, 0, sampleCount());
}