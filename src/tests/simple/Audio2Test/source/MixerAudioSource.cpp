//
// Created by Crs_1 on 2023/7/8.
//

#include "MixerAudioSource.h"
#include "MixerAudioSource_p.h"

#include "buffer/AudioBuffer.h"

MixerAudioSource::MixerAudioSource(): MixerAudioSource(*new MixerAudioSourcePrivate) {
}

MixerAudioSource::MixerAudioSource(MixerAudioSourcePrivate &d): AudioSource(d) {
}

MixerAudioSource::~MixerAudioSource() {
    Q_D(MixerAudioSource);
    MixerAudioSource::close();
    d->deleteOwnedSources();
}

bool MixerAudioSource::open(qint64 bufferSize, double sampleRate) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if(d->start(bufferSize, sampleRate)) {
        d->tmpBuf.resize(2, bufferSize);
        return AudioSource::open(bufferSize, sampleRate);
    } else {
        return false;
    }
}

qint64 MixerAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(MixerAudioSource);
    QList<float> magnitude;
    qint64 readLength = readData.length;
    {
        QMutexLocker locker(&d->mutex);
        auto channelCount = readData.buffer->channelCount();
        if(d->tmpBuf.channelCount() < channelCount) d->tmpBuf.resize(channelCount);
        for(int i = 0; i < channelCount; i++) {
            readData.buffer->clear(i, readData.startPos, readLength);
        }
        auto gainLeftRight = applyGainAndPan(d->gain, d->pan);
        for(auto src: sources()) {
            readLength = std::min(readLength, src->read(AudioSourceReadData(&d->tmpBuf, 0, readLength)));
            for(int i = 0; i < channelCount; i++) {
                auto gain = i == 0 ? gainLeftRight.first : i == 1 ? gainLeftRight.second : d->gain;
                readData.buffer->addSampleRange(i, readData.startPos, readLength, d->tmpBuf, i, 0, gain);
            }
        }

        for(int i = 0; i < channelCount; i++) {
            magnitude.append(readData.buffer->magnitude(i, readData.startPos, readLength));
        }
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

void MixerAudioSource::close() {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    d->tmpBuf.resize(0, 0);
    AudioSource::close();
}

bool MixerAudioSource::addSource(AudioSource *src, bool takeOwnership) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if(src == this) return false;
    if(d->sourceDict.contains(src)) return false;
    d->sourceDict.append(src, takeOwnership);
    if(isOpen()) {
        return src->open(bufferSize(), sampleRate());
    }
    return true;
}
bool MixerAudioSource::removeSource(AudioSource *src) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if(d->sourceDict.remove(src)) {
        src->close();
        return true;
    }
    return false;
}
void MixerAudioSource::removeAllSource() {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    d->sourceDict.clear();
}
QList<AudioSource *> MixerAudioSource::sources() const {
    Q_D(const MixerAudioSource);
    return d->sourceDict.keys();
}
void MixerAudioSource::setGain(float gain) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->gain = gain;
}
float MixerAudioSource::gain() const {
    Q_D(const MixerAudioSource);
    return d->gain;
}
void MixerAudioSource::setPan(float pan) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->pan = pan;
}
float MixerAudioSource::pan() const {
    Q_D(const MixerAudioSource);
    return d->pan;
}
