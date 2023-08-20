//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioDataWrapper.h"
#include "AudioDataWrapper_p.h"

AudioDataWrapper::AudioDataWrapper(float *const *data, int channelCount, qint64 sampleCount, qint64 startPos): AudioDataWrapper(*new AudioDataWrapperPrivate) {
    Q_D(AudioDataWrapper);
    d->data = data;
    d->channelCount = channelCount;
    d->sampleCount = sampleCount;
    d->startPos = startPos;
}

float &AudioDataWrapper::sampleAt(int channel, qint64 pos) {
    Q_D(AudioDataWrapper);
    return d->data[channel][d->startPos + pos];
}
float AudioDataWrapper::constSampleAt(int channel, qint64 pos) const {
    Q_D(const AudioDataWrapper);
    return d->data[channel][d->startPos + pos];
}
int AudioDataWrapper::channelCount() const {
    Q_D(const AudioDataWrapper);
    return d->channelCount;
}
qint64 AudioDataWrapper::sampleCount() const {
    Q_D(const AudioDataWrapper);
    return d->sampleCount;
}

float *AudioDataWrapper::data(int channel) const {
    Q_D(const AudioDataWrapper);
    return d->data[channel] + d->startPos;
}

void AudioDataWrapper::reset(float *const *data, int channelCount, qint64 sampleCount, qint64 startPos) {
    Q_D(AudioDataWrapper);
    d->data = data;
    d->channelCount = channelCount;
    d->sampleCount = sampleCount;
    d->startPos = startPos;
}

AudioDataWrapper::AudioDataWrapper(AudioDataWrapperPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
float *AudioDataWrapper::writePointerTo(int channel, qint64 startPos) {
    Q_D(AudioDataWrapper);
    return d->data[channel] + startPos;
}
bool AudioDataWrapper::isContinuous() const {
    return true;
}
const float *AudioDataWrapper::readPointerTo(int channel, qint64 startPos) const {
    Q_D(const AudioDataWrapper);
    return d->data[channel] + startPos;
}
