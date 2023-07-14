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
        for (int i = 0; i < channelCount; i++) {
            readData.buffer->clear(i, readData.startPos, readData.length);
        }
        readLength = std::min(readData.length, bufferLength - nextReadPosition());
        AudioBuffer tmpBuf(channelCount, readLength);
        for (auto src : sources()) {
            src->read(&tmpBuf);
            for (int i = 0; i < channelCount; i++) {
                readData.buffer->addSampleRange(i, readData.startPos, readLength, tmpBuf, i, 0);
            }
        }
        applyGainAndPan({readData.buffer, readData.startPos, readLength}, d->gain, d->pan);

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
    PositionableAudioSource::close();
}
qint64 PositionableMixerAudioSource::length() const {
    auto sourceList = sources();
    return std::reduce(sourceList.constBegin(), sourceList.constEnd(), std::numeric_limits<qint64>::max(), [](qint64 l, PositionableAudioSource *src){
        return std::min(l, src->length());
    });
}

void PositionableMixerAudioSourcePrivate::setNextReadPositionToAll(qint64 pos) {
    auto sourceList = sourceDict.keys();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
        reinterpret_cast<PositionableAudioSource *>(src)->setNextReadPosition(pos);
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
    if(d->sourceDict.append(src, takeOwnership).second) {
        if(isOpened()) {
            src->open(bufferSize(), sampleRate());
            src->setNextReadPosition(nextReadPosition());
        }
        return true;
    }
    return false;
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

