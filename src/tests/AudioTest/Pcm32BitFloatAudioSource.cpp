//
// Created by Crs_1 on 2023/5/5.
//

#include "Pcm32BitFloatAudioSource.h"

static inline quint64 rawToBufSize(quint64 size, quint64 chCnt) {
    return size / chCnt / sizeof(float);
}

static inline quint64 bufSizeToRaw(quint64 size, quint64 chCnt) {
    return size * chCnt * sizeof(float);
}

quint64 Pcm32BitFloatAudioSource::_read(AudioBufferList &buf, quint64 size, quint64 offset, bool isPeeking) {
    auto rawBuf = new float[size * channelCount()];
    quint64 readSize;
    if(isPeeking) {
        readSize = file.peek((char*)rawBuf, size * channelCount() * sizeof(float));
    } else {
        readSize = file.read((char*)rawBuf, size * channelCount() * sizeof(float));
    }

    auto readSampleCount = rawToBufSize(readSize, channelCount());
    for(int i = 0; i < readSampleCount; i++) {
        for(int j = 0; j < channelCount(); j++) {
            buf[j][i] = rawBuf[i * channelCount() + j];
        }
    }
    return readSampleCount;
}
quint64 Pcm32BitFloatAudioSource::read(AudioBufferList &buf, quint64 size) {
    return _read(buf, size, 0, false);
}
quint64 Pcm32BitFloatAudioSource::peek(AudioBufferList &buf, quint64 size, quint64 offset) {
    return _read(buf, size, offset, true);
}
quint64 Pcm32BitFloatAudioSource::pos() const {
    return rawToBufSize(file.pos(), channelCount());
}
bool Pcm32BitFloatAudioSource::setPos(quint64 pos) {
    return file.seek(bufSizeToRaw(pos, channelCount()));
}
quint32 Pcm32BitFloatAudioSource::sampleRate() const {
    return m_sampleRate;
}
quint16 Pcm32BitFloatAudioSource::channelCount() const {
    return m_channelCount;
}
quint64 Pcm32BitFloatAudioSource::readableSampleCount() const {
    return file.bytesAvailable() / channelCount() / sizeof(float);
}
Pcm32BitFloatAudioSource::Pcm32BitFloatAudioSource(const QString &fileName, quint16 channelCount, quint32 sampleRate)
    : m_sampleRate(sampleRate), m_channelCount(channelCount), file(fileName) {

}
bool Pcm32BitFloatAudioSource::open() {
    return file.open(QIODevice::ReadOnly);
}
void Pcm32BitFloatAudioSource::close() {
    file.close();
}
bool Pcm32BitFloatAudioSource::isSequential() const {
    return false;
}
bool Pcm32BitFloatAudioSource::isSampleRateChangeable() const {
    return false;
}
