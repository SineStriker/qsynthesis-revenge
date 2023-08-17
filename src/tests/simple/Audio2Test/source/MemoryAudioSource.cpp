//
// Created by Crs_1 on 2023/7/8.
//

#include "MemoryAudioSource.h"
#include "MemoryAudioSource_p.h"

#include "buffer/IAudioSampleContainer.h"
#include <algorithm>

MemoryAudioSource::MemoryAudioSource(IAudioSampleProvider *buffer, bool takeOwnership): MemoryAudioSource(*new MemoryAudioSourcePrivate) {
    Q_D(MemoryAudioSource);
    d->buffer = buffer;
    d->takeOwnership = takeOwnership;
}

MemoryAudioSource::MemoryAudioSource(MemoryAudioSourcePrivate &d): PositionableAudioSource(d) {
}

MemoryAudioSource::~MemoryAudioSource() {
    Q_D(MemoryAudioSource);
    if(d->takeOwnership) {
        delete d->buffer;
    }
}

IAudioSampleProvider *MemoryAudioSource::buffer() const {
    Q_D(const MemoryAudioSource);
    return d->buffer;
}

IAudioSampleProvider *MemoryAudioSource::setBuffer(IAudioSampleProvider *newBuffer, bool takeOwnership) {
    Q_D(MemoryAudioSource);
    QMutexLocker locker(&d->mutex);
    auto oldBuffer = d->buffer;
    d->buffer = newBuffer;
    d->takeOwnership = takeOwnership;
    d->position = 0;
    return oldBuffer;
}

qint64 MemoryAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(MemoryAudioSource);
    QMutexLocker locker(&d->mutex);
    auto bufferLength = length();
    auto channelCount = std::min(d->buffer->channelCount(), readData.buffer->channelCount());
    auto readLength = std::min(readData.length, bufferLength - nextReadPosition());
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->setSampleRange(i, readData.startPos, readLength, *d->buffer, i, d->position);
    }
    d->position += readLength;
    return readLength;
}

qint64 MemoryAudioSource::length() const {
    Q_D(const MemoryAudioSource);
    return d->buffer->sampleCount();
}

void MemoryAudioSource::setNextReadPosition(qint64 pos) {
    Q_D(MemoryAudioSource);
    QMutexLocker locker(&d->mutex);
    PositionableAudioSource::setNextReadPosition(pos);
}
