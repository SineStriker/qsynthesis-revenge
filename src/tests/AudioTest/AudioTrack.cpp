//
// Created by Crs_1 on 2023/5/5.
//

#include <QDebug>

#include "AudioTrack.h"

quint64 AudioTrack::_read(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode, bool isPeeking) {
    if(buf->channelCount() < channelCount()) return -1;
    if(buf->channelCount() > channelCount()) {
        qWarning() << "Redundant buffer channels.";
    }
    if(buf->size() < size) return -1;
    if(buf->size() > size) {
        qWarning() << "Redundant buffer size.";
    }
    if(mode == Immediate) {
        for(auto src: sources()) {
            if(!src->canRead(size)) {
                size = src->readableSampleCount();
            }
        }
    }
    AudioBuffer tmpBuf(channelCount(), size);
    for(auto src: sources()) {
        if(isPeeking) {
            src->peek(&tmpBuf, size, mode);
        } else {
            src->read(&tmpBuf, size, mode);
        }
        for(int i = 0; i < channelCount(); i++) {
            for(int j = 0; j < size; j++) {
                buf->buffer(i)[j] += tmpBuf.buffer(i)[j];
            }
        }
    }
    return size;
}

quint64 AudioTrack::read(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode) {
    return _read(buf, size, mode, false);
}
quint64 AudioTrack::peek(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode) {
    return _read(buf, size, mode, true);
}
quint64 AudioTrack::pos() {
    return m_pos;
}
bool AudioTrack::setPos(quint64 pos) {
    m_pos = pos;
    bool flag = true;
    for(auto src: sources()) {
        flag = flag && src->setPos(pos);
    }
    return flag;
}
bool AudioTrack::isSequential() {
    return false;
}
quint32 AudioTrack::sampleRate() {
    return acceptableSampleRates()[0];
}
bool AudioTrack::setSampleRate(quint32 sampleRate) {
    if(!isSampleRateChangeable()) return false;
    for(auto src: sources()) {
        src->setSampleRate(sampleRate);
    }
    removeAcceptableSampleRate(acceptableSampleRates()[0]);
    addAcceptableSampleRate(sampleRate);
    return true;
}
bool AudioTrack::isSampleRateChangeable() {
    for(auto src: sources()) {
        if(!src->isSampleRateChangeable()) return false;
    };
    return true;
}
quint16 AudioTrack::channelCount() {
    return m_channelCount;
}
quint64 AudioTrack::readableSampleCount() {
    return 0;
}
bool AudioTrack::canRead(quint64 size) {
    return false;
}
AudioTrack::AudioTrack(quint16 channelCount, quint32 sampleRate): m_channelCount(channelCount) {
    addAcceptableSampleRate(sampleRate);
}
bool AudioTrack::addSource(IAudioSource *src) {
    if(src->channelCount() != m_channelCount) return false;
    if(src->sampleRate() != sampleRate() && !src->isSampleRateChangeable()) return false;
    if(src->isSequential()) return false;
    src->setSampleRate(sampleRate());
    src->setPos(pos());
    return AudioBus::addSource(src);
}
