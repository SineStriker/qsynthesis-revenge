//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioDataWrapper.h"
#include "AudioDataWrapper_p.h"

AudioDataWrapper::AudioDataWrapper(float *const *data, int channelCount, int sampleCount, int startPos): AudioDataWrapper(*new AudioDataWrapperPrivate) {
    Q_D(AudioDataWrapper);
    d->data = data;
    d->channelCount = channelCount;
    d->sampleCount = sampleCount;
    d->startPos = startPos;
}

float &AudioDataWrapper::sampleAt(int channel, int pos) {
    Q_D(AudioDataWrapper);
    return d->data[channel][d->startPos + pos];
}
float AudioDataWrapper::constSampleAt(int channel, int pos) const {
    Q_D(const AudioDataWrapper);
    return d->data[channel][d->startPos + pos];
}
int AudioDataWrapper::channelCount() const {
    Q_D(const AudioDataWrapper);
    return d->channelCount;
}
int AudioDataWrapper::sampleCount() const {
    Q_D(const AudioDataWrapper);
    return d->sampleCount;
}

float *const *AudioDataWrapper::data() const {
    Q_D(const AudioDataWrapper);
    return d->data;
}

void AudioDataWrapper::resetData(float *const *data, int channelCount, int sampleCount, int startPos) {
    Q_D(AudioDataWrapper);
    d->data = data;
    d->channelCount = channelCount;
    d->sampleCount = sampleCount;
    d->startPos = startPos;
}

AudioDataWrapper::AudioDataWrapper(AudioDataWrapperPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
