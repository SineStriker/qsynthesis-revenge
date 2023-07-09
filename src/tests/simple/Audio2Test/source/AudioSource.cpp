//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioSource.h"
#include "AudioSource_p.h"

#include "buffer/AudioBuffer.h"

AudioSourceReadData::AudioSourceReadData(IAudioSampleContainer *buffer): buffer(buffer), startPos(0), length(buffer->sampleCount()) {
}

AudioSourceReadData::AudioSourceReadData(IAudioSampleContainer *buffer, int startPos, int length): buffer(buffer), startPos(startPos), length(length) {
}

AudioSource::AudioSource(): AudioSource(*new AudioSourcePrivate) {
}

bool AudioSource::start(int bufferSize, double sampleRate) {
    Q_D(AudioSource);
    d->bufferSize = bufferSize;
    d->sampleRate = sampleRate;
    d->isStarted = true;
    return true;
}
bool AudioSource::isStarted() const {
    Q_D(const AudioSource);
    return d->isStarted;
}
void AudioSource::stop() {
    Q_D(AudioSource);
    d->bufferSize = 0;
    d->sampleRate = 0;
    d->isStarted = false;
}
int AudioSource::bufferSize() const {
    Q_D(const AudioSource);
    return d->bufferSize;
}
double AudioSource::sampleRate() const {
    Q_D(const AudioSource);
    return d->sampleRate;
}
AudioSource::AudioSource(AudioSourcePrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
AudioSource::~AudioSource() {
}
