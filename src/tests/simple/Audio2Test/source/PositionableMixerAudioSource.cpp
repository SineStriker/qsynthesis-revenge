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
    PositionableMixerAudioSource::close();
    d->deleteOwnedSources();
}

bool PositionableMixerAudioSource::open(qint64 bufferSize, double sampleRate) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    if(d->start(bufferSize, sampleRate)) {
        d->setNextReadPositionToAll(d->position);
        d->tmpBuf.resize(2, bufferSize);
        return PositionableAudioSource::open(bufferSize, sampleRate);
    } else {
        return false;
    }
}
qint64 PositionableMixerAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(PositionableMixerAudioSource);
    QList<float> magnitude;
    qint64 readLength;
    {
        QMutexLocker locker(&d->mutex);
        auto bufferLength = length();
        auto channelCount = readData.buffer->channelCount();
        if(d->tmpBuf.channelCount() < channelCount) d->tmpBuf.resize(channelCount);
        for (int i = 0; i < channelCount; i++) {
            readData.buffer->clear(i, readData.startPos, readData.length);
        }
        readLength = std::min(readData.length, bufferLength - nextReadPosition());
        auto gainLeftRight = applyGainAndPan(d->gain, d->pan);
        for (auto src : sources()) {
            src->read(&d->tmpBuf);
            for (int i = 0; i < channelCount; i++) {
                auto gain = i == 0 ? gainLeftRight.first : i == 1 ? gainLeftRight.second : d->gain;
                readData.buffer->addSampleRange(i, readData.startPos, readLength, d->tmpBuf, i, 0, gain);
            }
        }

        for (int i = 0; i < channelCount; i++) {
            magnitude.append(readData.buffer->magnitude(i, readData.startPos, readLength));
        }
        d->position += readLength;
    }

    float magL = 0, magR = 0;
    if(magnitude.length() >= 2) {
        magL = magnitude[0];
        magR = magnitude[1];
    } else if(magnitude.length() == 1) {
        magL = magR = magnitude[0];
    }
    emit meterUpdated(magL, magR);
    return readLength;
}
void PositionableMixerAudioSource::close() {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->tmpBuf.resize(0, 0);
    PositionableAudioSource::close();
}
qint64 PositionableMixerAudioSource::length() const {
    auto sourceList = sources();
    if(sourceList.length() == 0)
        return 0;
    return (*std::min_element(sourceList.begin(), sourceList.end(), [](PositionableAudioSource *src1, PositionableAudioSource *src2){
        return src1->length() < src2->length();
    }))->length();
}

void PositionableMixerAudioSourcePrivate::setNextReadPositionToAll(qint64 pos) {
    auto sourceList = sourceDict.keys();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](PositionableAudioSource *src){
        src->setNextReadPosition(pos);
    });
}

void PositionableMixerAudioSource::setNextReadPosition(qint64 pos) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->setNextReadPositionToAll(pos);
    PositionableAudioSource::setNextReadPosition(pos);
}
bool PositionableMixerAudioSource::addSource(PositionableAudioSource *src, bool takeOwnership) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if(src == this) return false;
    if(d->sourceDict.contains(src)) return false;
    d->sourceDict.append(src, takeOwnership);
    if(isOpen()) {
        if(!src->open(bufferSize(), sampleRate())) return false;
        src->setNextReadPosition(nextReadPosition());
    }
    return true;
}
bool PositionableMixerAudioSource::removeSource(PositionableAudioSource *src) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if (d->sourceDict.remove(src)) {
        src->close();
        return true;
    }
    return false;
}
void PositionableMixerAudioSource::removeAllSource() {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    d->sourceDict.clear();
}

QList<PositionableAudioSource *> PositionableMixerAudioSource::sources() const {
    Q_D(const PositionableMixerAudioSource);
    return d->sourceDict.keys();
}

void PositionableMixerAudioSource::setGain(float gain) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->gain = gain;
}
float PositionableMixerAudioSource::gain() const {
    Q_D(const PositionableMixerAudioSource);
    return d->gain;
}
void PositionableMixerAudioSource::setPan(float pan) {
    Q_D(PositionableMixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->pan = pan;
}
float PositionableMixerAudioSource::pan() const {
    Q_D(const PositionableMixerAudioSource);
    return d->pan;
}

