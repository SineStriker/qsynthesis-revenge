//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioBuffer.h"
AudioBuffer::AudioBuffer() {
}
AudioBuffer::AudioBuffer(int channelCount, qint64 sampleCount) {
    resize(channelCount, sampleCount);
}
float &AudioBuffer::sampleAt(int channel, qint64 pos) {
    return m_buffer[channel][pos];
}
float AudioBuffer::constSampleAt(int channel, qint64 pos) const {
    return m_buffer.at(channel).at(pos);
}
int AudioBuffer::channelCount() const {
    return m_buffer.size();
}
qint64 AudioBuffer::sampleCount() const {
    return m_buffer.empty() ? 0 : m_buffer[0].size();
}
void AudioBuffer::resize(int newChannelCount, qint64 newSampleCount) {
    if(newChannelCount != -1) {
        m_buffer.resize(newChannelCount);
        if(!m_buffer.empty())
            for(auto &vec: m_buffer) {
                vec.resize(m_buffer[0].size());
            }
    }
    if(newSampleCount != -1) {
        for(auto &vec: m_buffer) {
            vec.resize(newSampleCount);
        }
    }
}
float *AudioBuffer::data(int channel) {
    return m_buffer[channel].data();
}
float const *AudioBuffer::constData(int channel) const {
    return m_buffer.at(channel).data();
}
QVector<float> &AudioBuffer::vector(int channel) {
    return m_buffer[channel];
}
const QVector<float> &AudioBuffer::constVector(int channel) const {
    return m_buffer.at(channel);
}

template<typename T>
static inline QVector<T> vectorSlice(const QVector<T> &l, qint64 s, qint64 t = -1) {
    return QVector<T>(l.cbegin() + s, t == -1 ? l.cend() : l.cbegin() + s + t);
}

AudioBuffer AudioBuffer::slice(int startChannelIndex, qint64 startSampleCount, int channelSize, qint64 length) const {
    auto b = vectorSlice(m_buffer, startChannelIndex, channelSize);
    for(auto &vec: b) {
        vec = vectorSlice(vec, startSampleCount, length);
    }
    AudioBuffer newBuf;
    newBuf.m_buffer = std::move(b);
    return newBuf;
}

AudioBuffer AudioBuffer::from(const IAudioSampleProvider &src) {
    auto channelCount = src.channelCount();
    auto sampleCount = src.sampleCount();
    AudioBuffer buf(channelCount, sampleCount);
    buf.setSampleRange(src);
    return buf;
}
float *AudioBuffer::writePointerTo(int channel, qint64 startPos) {
    return m_buffer[channel].data() + startPos;
}
bool AudioBuffer::isContinuous() const {
    return true;
}
const float *AudioBuffer::readPointerTo(int channel, qint64 startPos) const {
    return m_buffer[channel].constData() + startPos;
}
