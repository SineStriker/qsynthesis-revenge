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
                buf[i][j] += tmpBuf[i][j] * m_volumeRates[i];
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
quint64 AudioTrack::pos() const {
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
quint32 AudioTrack::sampleRate() const {
    return acceptableSampleRate();
}
bool AudioTrack::setSampleRate(quint32 sampleRate) {
    return setAcceptableSampleRate(sampleRate);
}
bool AudioTrack::isSampleRateChangeable() const {
    return std::all_of(m_sources.begin(), m_sources.end(), [](auto src){
        return src->isSampleRateChangeable();
    });
}
quint16 AudioTrack::channelCount() const {
    return m_channelCount;
}
quint64 AudioTrack::readableSampleCount() const {
    return 0;
}
AudioTrack::AudioTrack(quint16 channelCount, quint32 sampleRate): m_channelCount(channelCount) {
    setAcceptableSampleRate(sampleRate);
    m_volumeRates.resize(channelCount);
    m_volumeRates.fill(1.0);
}
bool AudioTrack::addSource(IAudioSource *src) {
    if(src->channelCount() != m_channelCount) return false;
    if(src->isSequential()) return false;
    if(!src->setReadMode(m_readMode)) return false;
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
bool AudioTrack::isSequential() const {
    return false;
}
void AudioTrack::setVolume(const QVector<float> &rates) {
    for(int i = 0; i < m_volumeRates.size(); i++) {
        if(i >= rates.size()) break;
        m_volumeRates[i] = rates[i];
    }
}
void AudioTrack::setVolume(float rate) {
    m_volumeRates.fill(rate);
}
QVector<float> AudioTrack::volume() {
    return m_volumeRates;
}
IAudioSource::ReadMode AudioTrack::readMode() const {
    return m_readMode;
}
bool AudioTrack::setReadMode(ReadMode mode) {
    return std::all_of(m_sources.begin(), m_sources.end(), [=](auto src){
        return src->setReadMode(mode);
    });
}

