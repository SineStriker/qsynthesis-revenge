//
// Created by Crs_1 on 2023/5/5.
//

#include "AudioBus.h"

quint32 AudioBus::acceptableSampleRate() const {
    return m_acceptableSampleRate;
}
bool AudioBus::addSource(IAudioSource *src) {
    m_sources.append(src);
    return true;
}
QList<IAudioSource *> AudioBus::sources() const {
    return m_sources.values();
}
bool AudioBus::removeSource(IAudioSource *source) {
    return m_sources.remove(source);
}
void AudioBus::setAcceptableSampleRate(quint32 sampleRate) {
    m_acceptableSampleRate = sampleRate;
}
