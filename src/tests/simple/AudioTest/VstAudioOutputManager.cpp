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
        auto vstOutput = m_outputs[i];
        auto size = vstOutput->m_track->read(buf, param->bufferSize);
        for(int k = 0; k < CHANNEL_COUNT; k++) {
            ::memcpy(outputs[i][k], buf[k].data(), size * sizeof(float));
            ::memset(outputs[i][k] + size, 0, (param->bufferSize - size) * sizeof(float));
        }
    }
    return true;
}
bool VstAudioOutputManager::vstProcessInitializer(bool isOffline, double sampleRate) {
    if(!std::all_of(m_outputs.begin(), m_outputs.end(), [=](VstAudioOutput *output){
        return output->m_track->setReadMode(isOffline ? IAudioSource::Synchronous : IAudioSource::Immediate);
    })) return false;
    if(sampleRate != m_sampleRate) {
        if(!std::all_of(m_outputs.begin(), m_outputs.end(), [=](VstAudioOutput *output){
            output->setAcceptableSampleRate(sampleRate);
            return output->m_track->setSampleRate(sampleRate);
        })) return false;
        m_sampleRate = sampleRate;
    }
    return true;
}
