//
// Created by Crs_1 on 2023/5/10.
//

#include "VstAudioOutput.h"

const int CHANNEL_COUNT = 2;

VstAudioOutput::VstAudioOutput(QObject *parent) : QObject(parent) {
    m_track = new AudioTrack(CHANNEL_COUNT, 0);
}
bool VstAudioOutput::addSource(IAudioSource *src) {
    if(m_track->addSource(src)) {
        m_sources.append(src);
        return true;
    }
    return false;
}
bool VstAudioOutput::removeSource(IAudioSource *source) {
    if(m_track->removeSource(source)) {
        m_sources.remove(source);
        return true;
    }
    return false;
}
VstAudioOutput::~VstAudioOutput() {
    m_track->deleteLater();
}
