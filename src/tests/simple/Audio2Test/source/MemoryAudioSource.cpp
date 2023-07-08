//
// Created by Crs_1 on 2023/7/8.
//

#include "MemoryAudioSource.h"

#include "buffer/IAudioSampleContainer.h"
#include <algorithm>

MemoryAudioSource::MemoryAudioSource(IAudioSampleContainer *buffer, bool takeOwnership): m_buffer(buffer), m_takeOwnership(takeOwnership) {
}

MemoryAudioSource::~MemoryAudioSource() {
    if(m_takeOwnership) {
        delete m_buffer;
    }
}

IAudioSampleContainer *MemoryAudioSource::buffer() const {
    return m_buffer;
}

IAudioSampleContainer *MemoryAudioSource::resetBuffer(IAudioSampleContainer *newBuffer, bool takeOwnership) {
    auto oldBuffer = m_buffer;
    m_buffer = newBuffer;
    m_takeOwnership = takeOwnership;
    m_position = 0;
    return oldBuffer;
}

bool MemoryAudioSource::start(int bufferSize, double sampleRate) {
    m_isStarted = true;
    return true;
}
bool MemoryAudioSource::isStarted() const {
    return m_isStarted;
}
int MemoryAudioSource::read(const AudioSourceReadData &readData) {
    auto bufferLength = length();
    auto channelCount = std::min(m_buffer->channelCount(), readData.buffer->channelCount());
    auto readLength = std::min(readData.length, bufferLength - m_position);
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->setSampleRange(i, readData.startPos, readLength, *m_buffer, i, m_position);
    }
    m_position += readLength;
    return readLength;
}
void MemoryAudioSource::stop() {
    m_isStarted = false;
}
int MemoryAudioSource::length() const {
    return m_buffer->sampleCount();
}
int MemoryAudioSource::nextReadPosition() const {
    return m_position;
}
void MemoryAudioSource::setNextReadPosition(int pos) {
    m_position = pos;
}
