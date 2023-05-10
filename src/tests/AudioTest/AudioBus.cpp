//
// Created by Crs_1 on 2023/5/5.
//

#include "AudioBus.h"

quint32 AudioBus::acceptableSampleRate() const {
    return m_acceptableSampleRate;
}
bool AudioBus::addSource(IAudioSource *src) {
    if(src->m_bus != nullptr) return false;
    if(m_acceptableSampleRate != 0 && src->sampleRate() != m_acceptableSampleRate) {
        if(!src->setSampleRate(m_acceptableSampleRate)) return false;
    }
    m_sources.append(src);
    src->m_bus = this;
    return true;
}
QList<IAudioSource *> AudioBus::sources() const {
    return m_sources.values();
}
bool AudioBus::removeSource(IAudioSource *src) {
    if(m_sources.remove(src)) {
        src->m_bus = nullptr;
        return true;
    }
    return false;
}
bool AudioBus::setAcceptableSampleRate(quint32 sampleRate) {
    for(auto src: m_sources) {
        if(!src->isSampleRateChangeable()) return false;
    }
    for(auto src: m_sources) {
        if(!src->setSampleRate(sampleRate)) return false;
    }
    m_acceptableSampleRate = sampleRate;
    return true;
}
