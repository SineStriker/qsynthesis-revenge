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

}

bool MixerAudioSource::start(qint64 bufferSize, double sampleRate) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    if(d->start(bufferSize, sampleRate)) {
        return AudioSource::start(bufferSize, sampleRate);
    } else {
        return false;
    }
}

qint64 MixerAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    auto channelCount = readData.buffer->channelCount();
    AudioBuffer tmpBuf(channelCount, readData.length);
    qint64 readLength = readData.length;
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->clear(i, readData.startPos, readLength);
    }
    for(auto src: sources()) {
        readLength = std::min(readLength, src->read(AudioSourceReadData(&tmpBuf, 0, readLength)));
        for(int i = 0; i < channelCount; i++) {
            readData.buffer->addSampleRange(i, readData.startPos, readLength, tmpBuf, i, 0);
        }
    }
    return readLength;
}

void MixerAudioSource::stop() {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    AudioSource::stop();
}

void MixerAudioSource::addSource(AudioSource *src, bool takeOwnership) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->sourceDict.append(src, takeOwnership);
}
void MixerAudioSource::removeSource(AudioSource *src) {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->sourceDict.remove(src);
}
void MixerAudioSource::removeAllSource() {
    Q_D(MixerAudioSource);
    QMutexLocker locker(&d->mutex);
    d->sourceDict.clear();
}
QList<AudioSource *> MixerAudioSource::sources() const {
    Q_D(const MixerAudioSource);
    return d->sourceDict.keys();
}

void IMixer::deleteOwnedSources() const {
    for(auto src: sourceDict.keys()) {
        if(sourceDict.value(src)) {
            delete src;
        }
    }
}
bool IMixer::start(qint64 bufferSize, double sampleRate) const {
    auto sourceList = sourceDict.keys();
    if(std::all_of(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
            return src->start(bufferSize, sampleRate);
        })) {
        return true;
    } else {
        return false;
    }
}
void IMixer::stop() const {
    auto sourceList = sourceDict.keys();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
        src->stop();
    });
}
