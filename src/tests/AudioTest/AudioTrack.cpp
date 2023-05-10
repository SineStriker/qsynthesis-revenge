//
// Created by Crs_1 on 2023/5/5.
//

#include <QDebug>

#include "AudioTrack.h"

quint64 AudioTrack::_read(AudioBufferList &buf, quint64 size, quint64 offset, bool isPeeking) {
    if(buf.size() != channelCount()) {
        return -1;
    }
    if(buf.bufferSize() < size) {
        return -1;
    }

    AudioBufferList tmpBuf(channelCount(), size);
    for(auto src: m_sources) {
        if(isPeeking) {
            size = std::min(size, src->peek(tmpBuf, size, offset));
        } else {
            size = std::min(size, src->read(tmpBuf, size));
        }
        for(int i = 0; i < channelCount(); i++) {
            for(int j = 0; j < size; j++) {
                buf[i][j] += tmpBuf[i][j];
            }
        }
    }
    emit audioRead(buf, size);
    return size;
}

quint64 AudioTrack::read(AudioBufferList &buf, quint64 size) {
    return _read(buf, size, 0, false);
}
quint64 AudioTrack::peek(AudioBufferList &buf, quint64 size, quint64 offset) {
    return _read(buf, size, offset, true);
}
quint64 AudioTrack::pos() {
    return m_pos;
}
bool AudioTrack::setPos(quint64 pos) {
    if(std::all_of(m_sources.begin(), m_sources.end(), [=](auto src){
        return src->setPos(pos);
    })) {
        m_pos = pos;
        return true;
    }
    return false;
}
quint32 AudioTrack::sampleRate() {
    return acceptableSampleRate();
}
bool AudioTrack::setSampleRate(quint32 sampleRate) {
    if(!isSampleRateChangeable()) return false;
    for(auto src: sources()) {
        src->setSampleRate(sampleRate);
    }
    return setAcceptableSampleRate(sampleRate);
}
bool AudioTrack::isSampleRateChangeable() {
    return std::all_of(m_sources.begin(), m_sources.end(), [](auto src){
        return src->isSampleRateChangeable();
    });
}
quint16 AudioTrack::channelCount() {
    return m_channelCount;
}
quint64 AudioTrack::readableSampleCount() {
    return 0;
}
AudioTrack::AudioTrack(quint16 channelCount, quint32 sampleRate): m_channelCount(channelCount) {
    setAcceptableSampleRate(sampleRate);
}
bool AudioTrack::addSource(IAudioSource *src) {
    if(src->channelCount() != m_channelCount) return false;
    if(src->sampleRate() != sampleRate() && !src->isSampleRateChangeable()) return false;
    if(src->isSequential()) return false;
    src->setSampleRate(sampleRate());
    return AudioBus::addSource(src);
}
bool AudioTrack::open() {
    return std::all_of(m_sources.begin(), m_sources.end(), [=](auto src){
        return src->open();
    });
}
void AudioTrack::close() {
    std::for_each(m_sources.begin(), m_sources.end(), [](auto src){
        src->close();
    });
}
bool AudioTrack::isSequential() {
    return false;
}
