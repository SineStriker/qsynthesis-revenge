//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioSource.h"

#include "buffer/AudioBuffer.h"

AudioSourceReadData::AudioSourceReadData(IAudioSampleContainer *buffer): buffer(buffer), startPos(0), length(buffer->sampleCount()) {
}

AudioSourceReadData::AudioSourceReadData(IAudioSampleContainer *buffer, int startPos, int length): buffer(buffer), startPos(startPos), length(length) {
}
