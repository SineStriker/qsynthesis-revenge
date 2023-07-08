//
// Created by Crs_1 on 2023/7/9.
//

#include "PositionableMixerAudioSource.h"
#include "buffer/AudioBuffer.h"

PositionableMixerAudioSource::~PositionableMixerAudioSource() {
    for(auto src: sources()) {
        if(m_sourceDict.value(src)) {
            delete src;
        }
    }
}

bool PositionableMixerAudioSource::start(int bufferSize, double sampleRate) {
    auto sourceList = sources();
    return m_isStarted = std::all_of(sourceList.constBegin(), sourceList.constEnd(), [=](PositionableAudioSource *src){
        src->setNextReadPosition(m_position);
        return src->start(bufferSize, sampleRate);
    });
}
bool PositionableMixerAudioSource::isStarted() const {
    return m_isStarted;
}
int PositionableMixerAudioSource::read(const AudioSourceReadData &readData) {
    auto bufferLength = length();
    auto channelCount = readData.buffer->channelCount();
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->clear(i, readData.startPos, readData.length);
    }
    auto readLength = std::min(readData.length, bufferLength - m_position);
    AudioBuffer tmpBuf(channelCount, readLength);
    for(auto src: sources()) {
        src->read(&tmpBuf);
        for(int i = 0; i < channelCount; i++) {
            readData.buffer->addSampleRange(i, readData.startPos, readLength, tmpBuf, i, 0);
        }
    }
    return readLength;
}
void PositionableMixerAudioSource::stop() {
    auto sourceList = sources();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](PositionableAudioSource *src){
        src->stop();
    });
}
int PositionableMixerAudioSource::length() const {
    auto sourceList = sources();
    return std::reduce(sourceList.constBegin(), sourceList.constEnd(), std::numeric_limits<int>::max(), [](int l, PositionableAudioSource *src){
        return std::min(l, src->length());
    });
}
int PositionableMixerAudioSource::nextReadPosition() const {
    return m_position;
}
void PositionableMixerAudioSource::setNextReadPosition(int pos) {
    auto sourceList = sources();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](PositionableAudioSource *src){
        src->setNextReadPosition(pos);
    });
    m_position = pos;
}
void PositionableMixerAudioSource::addSource(PositionableAudioSource *src, bool takeOwnership) {
    m_sourceDict.append(src, takeOwnership);
}
void PositionableMixerAudioSource::removeSource(PositionableAudioSource *src) {
    m_sourceDict.remove(src);
}
void PositionableMixerAudioSource::removeAllSource() {
    m_sourceDict.clear();
}
QList<PositionableAudioSource *> PositionableMixerAudioSource::sources() const {
    return m_sourceDict.keys();
}
