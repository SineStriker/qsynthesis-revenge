//
// Created by Crs_1 on 2023/5/6.
//

#include "AudioResampler.h"
quint64 AudioResampler::read(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode) {
    return 0;
}
quint64 AudioResampler::peek(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode) {
    return 0;
}
quint64 AudioResampler::pos() {
    return m_src->pos(); //TODO compute
}
bool AudioResampler::setPos(quint64 pos) {
    return m_src->setPos(pos); //TODO compute
}
quint32 AudioResampler::sampleRate() {
    return m_sampleRate;
}
bool AudioResampler::setSampleRate(quint32 sampleRate) {
    m_sampleRate = sampleRate;
    return true;
}
bool AudioResampler::isSampleRateChangeable() {
    return true;
}
quint16 AudioResampler::channelCount() {
    return m_src->channelCount();
}
quint64 AudioResampler::readableSampleCount() {
    return 0;
}
bool AudioResampler::canRead(quint64 size) {
    //TODO compute
    return IAudioSource::canRead(size);
}
bool AudioResampler::open() {
    return m_src->open();
}
bool AudioResampler::close() {
    return m_src->close();
}
AudioResampler::AudioResampler(IAudioSource *src): m_src(src) {
}
