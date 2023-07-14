//
// Created by Crs_1 on 2023/7/11.
//

#include "TransportAudioSource.h"
#include "TransportAudioSource_p.h"

#include <QThread>
#include <QAbstractEventDispatcher>

#include "buffer/InterleavedAudioDataWrapper.h"
#include "format/AudioFormatIO.h"

TransportAudioSource::TransportAudioSource(QObject *parent) : TransportAudioSource(*new TransportAudioSourcePrivate, parent) {
}
TransportAudioSource::~TransportAudioSource() {
    TransportAudioSource::close();
}
TransportAudioSource::TransportAudioSource(TransportAudioSourcePrivate &d, QObject *parent): AudioSource(d), QObject(parent) {
}

static inline void safeRead(IAudioSampleContainer *dest, qint64 destPos, qint64 length, PositionableAudioSource *src) {
    src->read({dest, destPos, std::min(src->length() - src->nextReadPosition(), length)});
}

static inline bool inRange(qint64 x, qint64 l, qint64 r) {
    return x >= l && x < r;
}

qint64 TransportAudioSource::read(const AudioSourceReadData &readData) {
    Q_D(TransportAudioSource);
    if(d->position == d->loopingStart && d->position == d->loopingEnd) return 0;
    QMutexLocker locker(&d->mutex);
    int channelCount = readData.buffer->channelCount();
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->clear(i, readData.startPos, readData.length);
    }
    if(!d->isPlaying) return readData.length;
    if(d->src) {
        qint64 curBufPos = readData.startPos;
        qint64 lengthToRead = readData.length;
        qint64 srcPos = d->src->nextReadPosition();
        while(curBufPos + d->loopingEnd - srcPos < readData.startPos + readData.length && inRange(d->loopingEnd, srcPos, srcPos + lengthToRead)) {
            safeRead(readData.buffer, curBufPos, d->loopingEnd - srcPos, d->src);
            curBufPos += d->loopingEnd - srcPos;
            lengthToRead -= d->loopingEnd - srcPos;
            d->src->setNextReadPosition(d->loopingStart);
            d->position = d->loopingStart;
            d->_q_positionAboutToChange(d->loopingStart);
            srcPos = d->loopingStart;
        }
        safeRead(readData.buffer, curBufPos, lengthToRead, d->src);
    }
    d->position += readData.length;
    if(readData.length != 0) d->_q_positionAboutToChange(d->position + readData.length);
    return readData.length;
}
bool TransportAudioSource::open(qint64 bufferSize, double sampleRate) {
    Q_D(TransportAudioSource);
    QMutexLocker locker(&d->mutex);
    d->isPlaying = false;
    if(d->src && d->src->open(bufferSize, sampleRate)) {
        d->isPlaying = false;
        return IAudioStream::open(bufferSize, sampleRate);
    }
    return false;
}
void TransportAudioSource::close() {
    Q_D(TransportAudioSource);
    if(d->src) d->src->close();
    d->isPlaying = false;
    IAudioStream::close();
}

void TransportAudioSource::setSource(PositionableAudioSource *src) {
    Q_D(TransportAudioSource);
    QMutexLocker locker(&d->mutex);
    auto curSrc = d->src;
    d->src = src;
    if(src) {
        if(isOpened()) {
            src->setNextReadPosition(d->position);
            src->open(bufferSize(), sampleRate());
        }
    }
}
PositionableAudioSource *TransportAudioSource::source() const {
    Q_D(const TransportAudioSource);
    return d->src;
}

void TransportAudioSource::play() {
    Q_D(TransportAudioSource);
    QMutexLocker locker(&d->mutex);
    d->isPlaying = true;
}
bool TransportAudioSource::isPlaying() const {
    Q_D(const TransportAudioSource);
    return d->isPlaying;
}
void TransportAudioSource::pause() {
    Q_D(TransportAudioSource);
    QMutexLocker locker(&d->mutex);
    d->isPlaying = false;
}
qint64 TransportAudioSource::position() const {
    Q_D(const TransportAudioSource);
    return d->position;
}
void TransportAudioSource::setPosition(qint64 position) {
    Q_D(TransportAudioSource);
    QMutexLocker locker(&d->mutex);
    if(position == d->position) return;
    d->_q_positionAboutToChange(position);
    d->position = position;
    if(d->src) d->src->setNextReadPosition(d->position);
}
qint64 TransportAudioSource::length() const {
    Q_D(const TransportAudioSource);
    if(d->src) return d->src->length();
    else return 0;
}
QPair<qint64, qint64> TransportAudioSource::loopingRange() const {
    Q_D(const TransportAudioSource);
    return {d->loopingStart, d->loopingEnd};
}
void TransportAudioSource::setLoopingRange(qint64 l, qint64 r) {
    Q_D(TransportAudioSource);
    QMutexLocker locker(&d->mutex);
    d->loopingStart = l;
    d->loopingEnd = r;
}

void TransportAudioSourcePrivate::_q_positionAboutToChange(qint64 pos) {
    Q_Q(TransportAudioSource);
    emit q->positionAboutToChange(pos);
}

void TransportAudioSource::lock() {
    Q_D(TransportAudioSource);
    d->mutex.lock();
}

void TransportAudioSource::unlock() {
    Q_D(TransportAudioSource);
    d->mutex.unlock();
}

TransportAudioSourceWriter::TransportAudioSourceWriter(TransportAudioSource *src, AudioFormatIO *outFile, qint64 length): src(src), outFile(outFile), length(length) {
}
void TransportAudioSourceWriter::start() {
    auto d = src->d_func();
    QMutexLocker locker(&d->mutex);
    qint64 curPos = d->position;
    qint64 blockSize = src->bufferSize();
    int chCnt = outFile->channels();
    auto *p = new float[blockSize * chCnt];
    memset(p, 0, sizeof(float) * blockSize * chCnt);
    InterleavedAudioDataWrapper buf(p, chCnt, blockSize);
    qint64 framesToRead = length;
    while(!stopRequested && framesToRead > 0) {
        qint64 readLength = std::min(framesToRead, blockSize);
        if(d->src) d->src->read({&buf, 0, readLength});
        framesToRead -= readLength;
        outFile->write(p, blockSize);
        d->_q_positionAboutToChange(d->position + readLength);
        d->position += readLength;
        emit percentageUpdated(100.0 * (length - framesToRead) / length);
    }
    delete[] p;
    d->_q_positionAboutToChange(curPos);
    d->position = curPos;
    if(d->src) d->src->setNextReadPosition(curPos);
    if(stopRequested) {
        emit interrupted();
    } else {
        emit finished();
    }
}
void TransportAudioSourceWriter::interrupt() {
    stopRequested = true;
}
