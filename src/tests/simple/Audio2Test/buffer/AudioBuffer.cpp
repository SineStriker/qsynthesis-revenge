//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioBuffer.h"
AudioBuffer::AudioBuffer() {
}
AudioBuffer::AudioBuffer(int channelCount, int sampleCount) {
    resize(channelCount, sampleCount);
}
float &AudioBuffer::sampleAt(int channel, int pos) {
    return m_buffer[channel][pos];
}
float AudioBuffer::constSampleAt(int channel, int pos) const {
    return m_buffer.at(channel).at(pos);
}
int AudioBuffer::channelCount() const {
    return m_buffer.size();
}
int AudioBuffer::sampleCount() const {
    return m_buffer.isEmpty() ? 0 : m_buffer[0].size();
}
void AudioBuffer::resize(int newChannelCount, int newSampleCount) {
    if(newChannelCount != -1) {
        m_buffer.resize(newChannelCount);
    }
    for(auto &vec: m_buffer) {
        vec.resize(newSampleCount);
    }
}
float *AudioBuffer::data(int channel) {
    return m_buffer[channel].data();
}
float const *AudioBuffer::constData(int channel) const {
    return m_buffer.at(channel).constData();
}
QVector<float> &AudioBuffer::vector(int channel) {
    return m_buffer[channel];
}
const QVector<float> &AudioBuffer::constVector(int channel) const {
    return m_buffer.at(channel);
}
AudioBuffer AudioBuffer::slice(int startChannelIndex, int startSampleCount, int channelSize, int length) const {
    auto b = m_buffer.mid(startChannelIndex, channelSize);
    for(auto &vec: b) {
        vec = vec.mid(startSampleCount, length);
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
