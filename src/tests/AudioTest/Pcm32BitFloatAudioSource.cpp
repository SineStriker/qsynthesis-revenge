//
// Created by Crs_1 on 2023/5/5.
//

#include "Pcm32BitFloatAudioSource.h"

quint64 Pcm32BitFloatAudioSource::_read(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode, bool isPeeking) {
    auto rawBuf = new float[size * channelCount()];
    quint64 readSize;
    if(isPeeking) {
        readSize = file.peek((char*)rawBuf, size * channelCount() * sizeof(float));
    } else {
        readSize = file.read((char*)rawBuf, size * channelCount() * sizeof(float));
    }

    auto readSampleCount = readSize / channelCount() / sizeof(float);
    if(mode == FillWithEmpty) {
        for(int i = 0; i < channelCount(); i++) {
            memset(buf->buffer(i), 0, readSampleCount * sizeof(float));
        }
    }
    for(int i = 0; i < readSampleCount; i++) {
        for(int j = 0; j < channelCount(); j++) {
            buf->buffer(j)[i] = rawBuf[i * channelCount() + j];
        }
    }
    return readSampleCount;
}
quint64 Pcm32BitFloatAudioSource::read(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode) {
    return _read(buf, size, mode, false);
}
quint64 Pcm32BitFloatAudioSource::peek(AudioBuffer *buf, quint64 size, IAudioSource::ReadMode mode) {
    return _read(buf, size, mode, true);
}
quint64 Pcm32BitFloatAudioSource::pos() {
    return file.pos() / channelCount() / sizeof(float);
}
bool Pcm32BitFloatAudioSource::setPos(quint64 pos) {
    return file.seek(pos * channelCount() * sizeof(float));
}
bool Pcm32BitFloatAudioSource::isSequential() {
    return false;
}
quint32 Pcm32BitFloatAudioSource::sampleRate() {
    return m_sampleRate;
}
bool Pcm32BitFloatAudioSource::setSampleRate(quint32 sampleRate) {
    return false;
}
bool Pcm32BitFloatAudioSource::isSampleRateChangeable() {
    return false;
}
quint16 Pcm32BitFloatAudioSource::channelCount() {
    return m_channelCount;
}
quint64 Pcm32BitFloatAudioSource::readableSampleCount() {
    return file.bytesAvailable() / channelCount() / sizeof(float);
}
bool Pcm32BitFloatAudioSource::canRead(quint64 size) {
    return readableSampleCount() >= size;
}
Pcm32BitFloatAudioSource::Pcm32BitFloatAudioSource(const QString &fileName, quint32 sampleRate, quint16 channelCount)
    : m_sampleRate(sampleRate), m_channelCount(channelCount) {
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
}
