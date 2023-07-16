//
// Created by Crs_1 on 2023/7/16.
//

#include "ChannelRoutingAudioSource.h"
#include "ChannelRoutingAudioSource_p.h"

ChannelRoutingAudioSource::ChannelRoutingAudioSource(): ChannelRoutingAudioSource(*new ChannelRoutingAudioSourcePrivate) {

}
ChannelRoutingAudioSource::~ChannelRoutingAudioSource() {
    Q_D(ChannelRoutingAudioSource);
    ChannelRoutingAudioSource::close();
    d->deleteOwnedSources();
}
ChannelRoutingAudioSource::ChannelRoutingAudioSource(ChannelRoutingAudioSourcePrivate &d): AudioSource(d) {
}
bool ChannelRoutingAudioSource::open(qint64 bufferSize, double sampleRate) {
    Q_D(ChannelRoutingAudioSource);
    QMutexLocker locker(&d->mutex);
    if(d->start(bufferSize, sampleRate)) {
        d->tmpBuf.resize(2, bufferSize);
        return AudioSource::open(bufferSize, sampleRate);
    } else {
        return false;
    }
}
qint64 ChannelRoutingAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(ChannelRoutingAudioSource);
    QMutexLocker locker(&d->mutex);
    qint64 readLength = readData.length;
    auto channelCount = readData.buffer->channelCount();
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->clear(i, readData.startPos, readLength);
    }
    int srcCnt = d->sourceDict.size();
    auto srcList = sources();
    for(int i = 0; i < channelCount / 2 && i < srcCnt; i++) {
        auto src = srcList[i];
        readLength = std::min(readLength, src->read(AudioSourceReadData(&d->tmpBuf, 0, readLength)));
        readData.buffer->setSampleRange(i * 2, readData.startPos, readLength, d->tmpBuf, 0, 0);
        readData.buffer->setSampleRange(i * 2 + 1, readData.startPos, readLength, d->tmpBuf, 1, 0);
    }
    return readLength;
}
void ChannelRoutingAudioSource::close() {
    Q_D(ChannelRoutingAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    d->tmpBuf.resize(0, 0);
    AudioSource::close();
}
bool ChannelRoutingAudioSource::addSource(AudioSource *src, bool takeOwnership) {
    Q_D(ChannelRoutingAudioSource);
    QMutexLocker locker(&d->mutex);
    if(src == this) return false;
    if(d->sourceDict.contains(src)) return false;
    d->sourceDict.append(src, takeOwnership);
    if(isOpen()) {
        return src->open(bufferSize(), sampleRate());
    }
    return true;
}
bool ChannelRoutingAudioSource::removeSource(AudioSource *src) {
    Q_D(ChannelRoutingAudioSource);
    QMutexLocker locker(&d->mutex);
    if(d->sourceDict.remove(src)) {
        src->close();
        return true;
    }
    return false;
}
void ChannelRoutingAudioSource::removeAllSource() {
    Q_D(ChannelRoutingAudioSource);
    QMutexLocker locker(&d->mutex);
    d->stop();
    d->sourceDict.clear();
}
QList<AudioSource *> ChannelRoutingAudioSource::sources() const {
    Q_D(const ChannelRoutingAudioSource);
    return d->sourceDict.keys();
}
