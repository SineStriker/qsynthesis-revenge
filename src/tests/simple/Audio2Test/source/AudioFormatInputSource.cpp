//
// Created by Crs_1 on 2023/7/12.
//

#include "AudioFormatInputSource.h"
#include "AudioFormatInputSource_p.h"

#include <QDebug>

#include "buffer/InterleavedAudioDataWrapper.h"
#include "format/AudioFormatIO.h"

AudioFormatInputSource::AudioFormatInputSource(AudioFormatIO *audioFormatIo, bool takeOwnership): AudioFormatInputSource(*new AudioFormatInputSourcePrivate) {
    setAudioFormatIo(audioFormatIo, takeOwnership);
}
AudioFormatInputSource::~AudioFormatInputSource() {
}
AudioFormatInputSource::AudioFormatInputSource(AudioFormatInputSourcePrivate &d): PositionableAudioSource(d) {
}

void AudioFormatInputSourcePrivate::resizeInDataBuffers(qint64 bufferSize) {
    inData.resize(io->channels() * (qint64)(bufferSize / ratio));
}
void AudioFormatInputSourcePrivate::resizeOutDataBuffers(qint64 bufferSize) {
    outData.resize(io->channels() * bufferSize);
}
long AudioFormatInputSourcePrivate::fetchInData(float **data) {
    Q_Q(AudioFormatInputSource);
    *data = inData.data();
    return io->read(inData.data(), q->bufferSize() / ratio);
}

qint64 AudioFormatInputSource::read(const AudioSourceReadData &readData) {
    Q_D(AudioFormatInputSource);
    auto readLength = std::min(readData.length, length() - d->position);
    if(!d->io) return readLength;
    if(readLength > bufferSize()) d->resizeOutDataBuffers(readLength);
    src_callback_read(d->srcState, d->ratio, readLength, d->outData.data());
    InterleavedAudioDataWrapper outBuf(d->outData.data(), d->io->channels(), readLength);
    int channelCount = std::min(d->io->channels(), readData.buffer->channelCount());
    for(int ch = 0; ch < channelCount; ch++) {
        readData.buffer->setSampleRange(ch, readData.startPos, readLength, outBuf, ch, 0);
    }
    d->position += readLength;
    return readLength;
}
qint64 AudioFormatInputSource::length() const {
    Q_D(const AudioFormatInputSource);
    if(!d->q_ptr || !isOpened()) return 0;
    return d->io->length() * d->ratio;
}
void AudioFormatInputSource::setNextReadPosition(qint64 pos) {
    Q_D(AudioFormatInputSource);
    src_reset(d->srcState);
    if(d->io) d->io->seek(pos / d->ratio);
    PositionableAudioSource::setNextReadPosition(pos);
}
bool AudioFormatInputSource::open(qint64 bufferSize, double sampleRate) {
    Q_D(AudioFormatInputSource);
    if(!d->io) return false;
    if(d->io->open(QIODevice::ReadOnly)) {
        d->ratio = sampleRate / d->io->sampleRate();
        int srcError = 0;
        d->srcState = src_callback_new([](void *cbData, float **data){
            return reinterpret_cast<AudioFormatInputSourcePrivate *>(cbData)->fetchInData(data);
        }, 0, d->io->channels(), &srcError, d);
        if(srcError) {
            qWarning() << src_strerror(srcError);
            return false;
        }
        srcError = src_set_ratio(d->srcState, d->ratio);
        if(srcError) {
            qWarning() << src_strerror(srcError);
            return false;
        }
        d->resizeInDataBuffers(bufferSize);
        d->resizeOutDataBuffers(bufferSize);
        d->io->seek(d->position / d->ratio);
        return IAudioStream::open(bufferSize, sampleRate);
    } else return false;
}
void AudioFormatInputSource::close() {
    Q_D(AudioFormatInputSource);
    if(!d->io) return;
    d->io->close();
    d->ratio = 0;
    if(d->srcState) src_delete(d->srcState);
    d->srcState = nullptr;
}

void AudioFormatInputSource::setAudioFormatIo(AudioFormatIO *audioFormatIo, bool takeOwnership) {
    if(isOpened()) {
        qWarning() << "Cannot set audio format io when source is opened.";
        return;
    }
    Q_D(AudioFormatInputSource);
    d->io = audioFormatIo;
    d->takeOwnership = takeOwnership;
    flush();
}
AudioFormatIO *AudioFormatInputSource::audioFormatIo() const {
    Q_D(const AudioFormatInputSource);
    return d->io;
}

void AudioFormatInputSource::flush() {
    Q_D(AudioFormatInputSource);
    if(d->srcState) src_reset(d->srcState);
}
