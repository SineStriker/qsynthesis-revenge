//
// Created by Crs_1 on 2023/5/5.
//

#include "AudioBuffer.h"

AudioBuffer::AudioBuffer(quint16 channelCount, quint64 size): m_isOwnedByInternal(true), m_size(size) {
    for(int i = 0; i < channelCount; i++) {
        auto buf = new float[size];
        std::fill(buf, buf + size, 0);
        m_buffers.append(buf);
    }
}
AudioBuffer::AudioBuffer(const QList<float *>& buffers): m_buffers(buffers), m_isOwnedByInternal(false), m_size(buffers.size()) {
}
AudioBuffer::~AudioBuffer() {
    if(m_isOwnedByInternal) {
        for(auto buf: m_buffers) {
            delete[] buf;
        }
    }
}
quint16 AudioBuffer::channelCount() const {
    return m_buffers.size();
}
float *AudioBuffer::buffer(quint16 channel) {
    if(channel >= m_size) return nullptr;
    return m_buffers[channel];
}
const float *AudioBuffer::constBuffer(quint16 channel) const {
    if(channel >= m_size) return nullptr;
    return m_buffers[channel];
}
quint64 AudioBuffer::size() const {
    return m_size;
}
void AudioBuffer::clear() {
    for(auto buf: m_buffers) {
        std::fill(buf, buf + m_size, 0);
    }
}
