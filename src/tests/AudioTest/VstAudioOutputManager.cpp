//
// Created by Crs_1 on 2023/5/10.
//

#include "VstAudioOutputManager.h"

const int OUTPUT_COUNT = 16;
const int CHANNEL_COUNT = 2;

VstAudioOutputManager::VstAudioOutputManager(QObject *parent) : QObject(parent) {
    for(int i = 0; i < OUTPUT_COUNT; i++) {
        m_outputs.append(new VstAudioOutput(this));
    }
}
VstAudioOutput *VstAudioOutputManager::output(int index) {
    return m_outputs[index];
}
bool VstAudioOutputManager::vstProcess(const VstAudioOutputManager::PlaybackParameters *param, bool isPlaying, qint32 outputCount, float *const *const *outputs) {
    AudioBufferList buf(CHANNEL_COUNT, param->bufferSize);
    for(int i = 0; i < OUTPUT_COUNT; i++) {
        buf.clearBuffer();
        quint64 size = param->bufferSize;
        auto vstOutput = m_outputs[i];
        AudioBufferList tmpBuf(CHANNEL_COUNT, param->bufferSize);
        for(auto src: vstOutput->m_sources) {
            size = std::min(size, src->read(tmpBuf, size));
            if(size < 0) return false;
            for(int k = 0; k < CHANNEL_COUNT; k++) for(int j = 0; j < size; j++) buf[k][j] += tmpBuf[k][j];
        }
        for(int k = 0; k < CHANNEL_COUNT; k++) {
            ::memcpy(outputs[i][k], buf[k].data(), size * sizeof(float));
            ::memset(outputs[i][k] + size, 0, (param->bufferSize - size) * sizeof(float));
        }
    }
    return true;
}
bool VstAudioOutputManager::vstProcessInitializer(bool isOffline, double sampleRate) {
    if(!std::all_of(m_outputs.begin(), m_outputs.end(), [=](VstAudioOutput *output){
        return std::all_of(output->m_sources.begin(), output->m_sources.end(), [=](IAudioSource *src){
            return src->setReadMode(isOffline ? IAudioSource::Synchronous : IAudioSource::Immediate);
        });
    })) return false;
    if(sampleRate != m_sampleRate) {
        if(!std::all_of(m_outputs.begin(), m_outputs.end(), [=](VstAudioOutput *output){
            return output->setAcceptableSampleRate(sampleRate);
        })) return false;
        m_sampleRate = sampleRate;
    }
    return true;
}
