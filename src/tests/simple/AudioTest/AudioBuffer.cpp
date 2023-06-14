//
// Created by Crs_1 on 2023/5/5.
//

#include "AudioBuffer.h"


AudioBufferList::AudioBufferList(quint16 channelCount, quint64 size) {
    QVector::resize(channelCount);
    resizeBuffer(size);
}
quint64 AudioBufferList::bufferSize() const {
    QVector<int> bufSizeList(size());
    std::transform(this->begin(), this->end(), bufSizeList.begin(), [](const auto &buf){ return buf.size(); });
    return *std::min(bufSizeList.begin(), bufSizeList.end());
}
void AudioBufferList::clearBuffer() {
    for(auto &buf: *this) {
        buf.fill(0.0);
    }
}
void AudioBufferList::resize(int asize) {
    auto prevSize = size();
    auto bufSize = bufferSize();
    QVector::resize(asize);
    resizeBuffer(bufSize);
}

void AudioBufferList::resizeBuffer(quint64 size) {
    for(auto &buf: *this) {
        buf.resize(size);
    }
}
