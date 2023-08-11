//
// Created by Crs_1 on 2023/7/9.
//

#include "SineWaveAudioSource.h"
#include "SineWaveAudioSource_p.h"

#include <cmath>

SineWaveAudioSource::SineWaveAudioSource(): SineWaveAudioSource(*new SineWaveAudioSourcePrivate) {
}
SineWaveAudioSource::SineWaveAudioSource(double frequency): SineWaveAudioSource() {
    Q_D(SineWaveAudioSource);
    d->freq = frequency;
}
SineWaveAudioSource::SineWaveAudioSource(SineWaveAudioSourcePrivate &d): PositionableAudioSource(d) {
}

bool SineWaveAudioSource::open(qint64 bufferSize, double sampleRate) {
    return AudioSource::open(bufferSize, sampleRate);
}

qint64 SineWaveAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(SineWaveAudioSource);
    QMutexLocker locker(&d->mutex);
    auto channelCount = readData.buffer->channelCount();
    auto pos = PositionableAudioSource::nextReadPosition();
    double omega = 2 * 3.14159265358979323846 * d->freq / sampleRate();
    for(qint64 i = 0; i < readData.length; i++) {
        float sample = sin(omega * (pos + i));
        for(int ch = 0; ch < channelCount; ch++) {
            readData.buffer->sampleAt(ch, readData.startPos + i) = sample;
        }
    }
    setNextReadPosition(pos + readData.length);
    return readData.length;
}
qint64 SineWaveAudioSource::length() const {
    return std::numeric_limits<qint64>::max();
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
