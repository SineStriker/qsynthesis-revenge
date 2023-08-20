//
// Created by Crs_1 on 2023/7/11.
//

#include "InterleavedAudioDataWrapper.h"
#include "InterleavedAudioDataWrapper_p.h"

InterleavedAudioDataWrapper::InterleavedAudioDataWrapper(float *data, int channelCount, qint64 sampleCount): InterleavedAudioDataWrapper(*new InterleavedAudioDataWrapperPrivate) {
    Q_D(InterleavedAudioDataWrapper);
    d->data = data;
    d->channelCount = channelCount;
    d->sampleCount = sampleCount;
}
InterleavedAudioDataWrapper::~InterleavedAudioDataWrapper() {
}
float &InterleavedAudioDataWrapper::sampleAt(int channel, qint64 pos) {
    Q_D(InterleavedAudioDataWrapper);
    return d->data[pos * d->channelCount + channel];
}
float InterleavedAudioDataWrapper::constSampleAt(int channel, qint64 pos) const {
    Q_D(const InterleavedAudioDataWrapper);
    return d->data[pos * d->channelCount + channel];
}
int InterleavedAudioDataWrapper::channelCount() const {
    Q_D(const InterleavedAudioDataWrapper);
    return d->channelCount;
}
qint64 InterleavedAudioDataWrapper::sampleCount() const {
    Q_D(const InterleavedAudioDataWrapper);
    return d->sampleCount;
}
float *InterleavedAudioDataWrapper::data() const {
    Q_D(const InterleavedAudioDataWrapper);
    return d->data;
}
void InterleavedAudioDataWrapper::resetData(float *data, int channelCount, qint64 sampleCount) {
    Q_D(InterleavedAudioDataWrapper);
    d->data = data;
    d->channelCount = channelCount;
    d->sampleCount = sampleCount;
}
InterleavedAudioDataWrapper::InterleavedAudioDataWrapper(InterleavedAudioDataWrapperPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}

float *InterleavedAudioDataWrapper::writePointerTo(int channel, qint64 startPos) {
    Q_D(InterleavedAudioDataWrapper);
    if(d->channelCount == 1 && channel == 0)
        return d->data + startPos;
    return nullptr;
}
bool InterleavedAudioDataWrapper::isContinuous() const {
    Q_D(const InterleavedAudioDataWrapper);
    return d->channelCount == 1;
}
const float *InterleavedAudioDataWrapper::readPointerTo(int channel, qint64 startPos) const {
    Q_D(const InterleavedAudioDataWrapper);
    if(d->channelCount == 1 && channel == 0)
        return d->data + startPos;
    return nullptr;
}
