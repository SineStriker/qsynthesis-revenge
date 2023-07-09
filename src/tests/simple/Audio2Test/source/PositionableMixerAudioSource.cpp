//
// Created by Crs_1 on 2023/7/9.
//

#include "PositionableMixerAudioSource.h"
#include "PositionableMixerAudioSource_p.h"

#include "buffer/AudioBuffer.h"

PositionableMixerAudioSource::PositionableMixerAudioSource(): PositionableMixerAudioSource(*new PositionableMixerAudioSourcePrivate) {
}
PositionableMixerAudioSource::PositionableMixerAudioSource(PositionableMixerAudioSourcePrivate &d): PositionableAudioSource(d) {
}

PositionableMixerAudioSource::~PositionableMixerAudioSource() {
    Q_D(PositionableMixerAudioSource);
    d->deleteOwnedSources();
}

bool PositionableMixerAudioSource::start(int bufferSize, double sampleRate) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if(d->start(bufferSize, sampleRate)) {
        d->setNextReadPosition(d->position);
        return PositionableAudioSource::start(bufferSize, sampleRate);
    } else {
        return false;
    }
}
int PositionableMixerAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    auto bufferLength = length();
    auto channelCount = readData.buffer->channelCount();
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->clear(i, readData.startPos, readData.length);
    }
    auto readLength = std::min(readData.length, bufferLength - nextReadPosition());
    AudioBuffer tmpBuf(channelCount, readLength);
    for(auto src: sources()) {
        src->read(&tmpBuf);
        for(int i = 0; i < channelCount; i++) {
            readData.buffer->addSampleRange(i, readData.startPos, readLength, tmpBuf, i, 0);
        }
    }
    d->position += readLength;
    return readLength;
}
void PositionableMixerAudioSource::stop() {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    PositionableAudioSource::stop();
}
int PositionableMixerAudioSource::length() const {
    auto sourceList = sources();
    return std::reduce(sourceList.constBegin(), sourceList.constEnd(), std::numeric_limits<int>::max(), [](int l, PositionableAudioSource *src){
        return std::min(l, src->length());
    });
}

void PositionableMixerAudioSourcePrivate::setNextReadPosition(int pos) {
    auto sourceList = sourceDict.keys();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
        reinterpret_cast<PositionableAudioSource *>(src)->setNextReadPosition(pos);
    });
}

void PositionableMixerAudioSource::setNextReadPosition(int pos) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->setNextReadPosition(pos);
    PositionableAudioSource::setNextReadPosition(pos);
}
void PositionableMixerAudioSource::addSource(PositionableAudioSource *src, bool takeOwnership) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->sourceDict.append(src, takeOwnership);
}
void PositionableMixerAudioSource::removeSource(PositionableAudioSource *src) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->sourceDict.remove(src);
}
void PositionableMixerAudioSource::removeAllSource() {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->sourceDict.clear();
}

QList<PositionableAudioSource *> PositionableMixerAudioSource::sources() const {
    Q_D(const PositionableMixerAudioSource);
    auto sourceList = d->sourceDict.keys();
    return *reinterpret_cast<QList<PositionableAudioSource *> *>(&sourceList); // Don't worry, this is strictly ok.
}


