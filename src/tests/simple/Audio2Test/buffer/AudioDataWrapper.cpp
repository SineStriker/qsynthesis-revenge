//
// Created by Crs_1 on 2023/7/8.
//

#include "AudioDataWrapper.h"

AudioDataWrapper::AudioDataWrapper(float *const *data, int channelCount, int sampleCount, int startPos): m_data(data), m_channelCount(channelCount), m_sampleCount(sampleCount), m_startPos(startPos) {
}

float &AudioDataWrapper::sampleAt(int channel, int pos) {
    return m_data[channel][m_startPos + pos];
}
float AudioDataWrapper::constSampleAt(int channel, int pos) const {
    return m_data[channel][m_startPos + pos];
}
int AudioDataWrapper::channelCount() const {
    return m_channelCount;
}
int AudioDataWrapper::sampleCount() const {
    return m_sampleCount;
}

float *const *AudioDataWrapper::data() const {
    return m_data;
}

void AudioDataWrapper::resetData(float *const *data, int channelCount, int sampleCount, int startPos) {
    m_data = data;
    m_channelCount = channelCount;
    m_sampleCount = sampleCount;
    m_startPos = startPos;
}
