//
// Created by Crs_1 on 2023/7/8.
//

#include "IAudioSampleContainer.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>

static inline void boundCheck(const IAudioSampleProvider &iAudioStorage, int channel, qint64 startPos, qint64 length) {
    assert(channel >= 0 && channel < iAudioStorage.channelCount());
    assert(startPos >= 0 && startPos <= iAudioStorage.sampleCount());
    assert(startPos + length >= 0 && startPos + length <= iAudioStorage.sampleCount());
}

float *IAudioSampleContainer::writePointerTo(int channel, qint64 startPos) {
    return nullptr;
}

void IAudioSampleContainer::setSampleRange(int destChannel, qint64 destStartPos, qint64 length,
                                           const IAudioSampleProvider &src,
                                   int srcChannel, qint64 srcStartPos) {
    boundCheck(*this, destChannel, destStartPos, length);
    boundCheck(src, srcChannel, srcStartPos, length);
    if(isContinuous() && src.isContinuous()) {
        auto pDest = writePointerTo(destChannel, destStartPos);
        auto pSrc = src.readPointerTo(srcChannel, srcStartPos);
        std::copy(pSrc, pSrc + length, pDest);
    } else {
        for(qint64 i = 0; i < length; i++) {
            sampleAt(destChannel, destStartPos + i) = src.constSampleAt(srcChannel, srcStartPos + i);
        }
    }
}
void IAudioSampleContainer::setSampleRange(const IAudioSampleProvider &src) {
    auto minChannelCount = std::min(channelCount(), src.channelCount());
    auto minSampleCount = std::min(sampleCount(), src.sampleCount());
    for(int i = 0; i < minChannelCount; i++) {
        setSampleRange(i, 0, minSampleCount, src, i, 0);
    }
}

void IAudioSampleContainer::addSampleRange(int destChannel, qint64 destStartPos, qint64 length,
                                           const IAudioSampleProvider &src,
                                   int srcChannel, qint64 srcStartPos, float gain) {
    boundCheck(*this, destChannel, destStartPos, length);
    boundCheck(src, srcChannel, srcStartPos, length);
    if(isContinuous() && src.isContinuous()) {
        auto pDest = writePointerTo(destChannel, destStartPos);
        auto pSrc = src.readPointerTo(srcChannel, srcStartPos);
        std::transform(pDest, pDest + length, pSrc, pDest, std::plus());
    } else {
        for(qint64 i = 0; i < length; i++) {
            sampleAt(destChannel, destStartPos + i) += src.constSampleAt(srcChannel, srcStartPos + i) * gain;
        }
    }

}
void IAudioSampleContainer::addSampleRange(const IAudioSampleProvider &src, float gain) {
    auto minChannelCount = std::min(channelCount(), src.channelCount());
    auto minSampleCount = std::min(sampleCount(), src.sampleCount());
    for(int i = 0; i < minChannelCount; i++) {
        addSampleRange(i, 0, minSampleCount, src, i, 0, gain);
    }
}

void IAudioSampleContainer::gainSampleRange(int destChannel, qint64 destStartPos, qint64 length, float gain) {
    boundCheck(*this, destChannel, destStartPos, length);
    if(isContinuous()) {
        auto p = writePointerTo(destChannel, destStartPos);
        std::transform(p, p + length, p, [gain](float num){
            return num * gain;
        });
    } else {
        for(qint64 i = 0; i < length; i++) {
            sampleAt(destChannel, destStartPos + i) *= gain;
        }
    }
}
void IAudioSampleContainer::gainSampleRange(int destChannel, float gain) {
    gainSampleRange(destChannel, 0, sampleCount(), gain);
}
void IAudioSampleContainer::gainSampleRange(float gain) {
    auto destChannelCount = channelCount();
    auto destSampleCount = sampleCount();
    for(int i = 0; i < destChannelCount; i++) {
        gainSampleRange(i, 0, destSampleCount, gain);
    }
}

void IAudioSampleContainer::clear(int destChannel, qint64 destStartPos, qint64 length) {
    boundCheck(*this, destChannel, destStartPos, length);
    if(isContinuous()) {
        memset(writePointerTo(destChannel, destStartPos), 0, length * sizeof(float));
    } else {
        for(qint64 i = 0; i < length; i++) {
            sampleAt(destChannel, destStartPos + i) = 0;
        }
    }
}
void IAudioSampleContainer::clear(int destChannel) {
    clear(destChannel, 0, sampleCount());
}
void IAudioSampleContainer::clear() {
    auto destChannelCount = channelCount();
    auto destSampleCount = sampleCount();
    for(int i = 0; i < destChannelCount; i++) {
        clear(i, 0, destSampleCount);
    }
}
