//
// Created by Crs_1 on 2023/5/5.
//

#include "AudioBus.h"

QList<quint32> AudioBus::acceptableSampleRates() {
    return m_acceptableSampleRates.values();
}
bool AudioBus::canAcceptSampleRate(quint32 sampleRate) {
    return m_acceptableSampleRates.contains(sampleRate);
}
bool AudioBus::addSource(IAudioSource *src) {
    m_sources.append(src);
    return true;
}
QList<IAudioSource *> AudioBus::sources() {
    return m_sources.values();
}
bool AudioBus::removeSource(IAudioSource *source) {
    return m_sources.remove(source);
}
void AudioBus::addAcceptableSampleRate(quint32 sampleRate) {
    m_acceptableSampleRates.insert(sampleRate);
}
void AudioBus::removeAcceptableSampleRate(quint32 sampleRate) {
    m_acceptableSampleRates.remove(sampleRate);
}
