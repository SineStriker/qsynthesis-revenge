//
// Created by Crs_1 on 2023/7/8.
//

#include "MixerAudioSource.h"
#include "buffer/AudioBuffer.h"

MixerAudioSource::~MixerAudioSource() {
    for(auto src: sources()) {
        if(m_sourceDict.value(src)) {
            delete src;
        }
    }
}

bool MixerAudioSource::start(int bufferSize, double sampleRate) {
    auto sourceList = sources();
    return m_isStarted = std::all_of(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
        return src->start(bufferSize, sampleRate);
    });
}
bool MixerAudioSource::isStarted() const {
    return m_isStarted;
}
int MixerAudioSource::read(const AudioSourceReadData &readData) {
    auto channelCount = readData.buffer->channelCount();
    AudioBuffer tmpBuf(channelCount, readData.length);
    int readLength = readData.length;
    for(int i = 0; i < channelCount; i++) {
        readData.buffer->clear(i, readData.startPos, readLength);
    }
    for(auto src: sources()) {
        readLength = std::min(readLength, src->read(AudioSourceReadData(&tmpBuf, 0, readLength)));
        for(int i = 0; i < channelCount; i++) {
            readData.buffer->addSampleRange(i, readData.startPos, readLength, tmpBuf, i, 0);
        }
    }
    return readLength;
}
void MixerAudioSource::stop() {
    auto sourceList = sources();
    std::for_each(sourceList.constBegin(), sourceList.constEnd(), [=](AudioSource *src){
        src->stop();
    });
}

void MixerAudioSource::addSource(AudioSource *src, bool takeOwnership) {
    m_sourceDict.append(src, takeOwnership);
}
void MixerAudioSource::removeSource(AudioSource *src) {
    m_sourceDict.remove(src);
}
void MixerAudioSource::removeAllSource() {
    m_sourceDict.clear();
}
QList<AudioSource *> MixerAudioSource::sources() const {
    return m_sourceDict.keys();
}
