//
// Created by Crs_1 on 2023/7/9.
//

#include "SineWaveAudioSource.h"
#include "SineWaveAudioSource_p.h"

SineWaveAudioSource::SineWaveAudioSource(): SineWaveAudioSource(*new SineWaveAudioSourcePrivate) {
}
SineWaveAudioSource::SineWaveAudioSource(double frequency): SineWaveAudioSource() {
    Q_D(SineWaveAudioSource);
    d->freq = frequency;
}
SineWaveAudioSource::SineWaveAudioSource(SineWaveAudioSourcePrivate &d): PositionableAudioSource(d) {
}

bool SineWaveAudioSource::start(int bufferSize, double sampleRate) {
    return AudioSource::start(bufferSize, sampleRate);
}

int SineWaveAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(SineWaveAudioSource);
    QMutexLocker locker(&d->mutex);
    auto channelCount = readData.buffer->channelCount();
    auto pos = PositionableAudioSource::nextReadPosition();
    double omega = 2 * 3.14159265358979323846 * d->freq / d->sampleRate;
    for(int i = 0; i < readData.length; i++) {
        float sample = sin(omega * (pos + i));
        for(int ch = 0; ch < channelCount; ch++) {
            readData.buffer->sampleAt(ch, readData.startPos + i) = sample;
        }
    }
    setNextReadPosition(pos + readData.length);
    return readData.length;
}
int SineWaveAudioSource::length() const {
    return std::numeric_limits<int>::max();
}

void SineWaveAudioSource::setFrequency(double frequency) {
    Q_D(SineWaveAudioSource);
    QMutexLocker locker(&d->mutex);
    d->freq = frequency;
}
double SineWaveAudioSource::frequency() const {
    Q_D(const SineWaveAudioSource);
    return d->freq;
}
