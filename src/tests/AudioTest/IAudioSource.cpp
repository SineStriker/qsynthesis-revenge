//
// Created by Crs_1 on 2023/5/6.
//

#include "IAudioSource.h"

bool IAudioSource::setPos(quint64 pos) {
    return false;
}
bool IAudioSource::isSequential() {
    return false;
}
bool IAudioSource::setSampleRate(quint32 sampleRate) {
    return false;
}
bool IAudioSource::isSampleRateChangeable() {
    return false;
}
bool IAudioSource::canRead(quint64 size) {
    return size <= readableSampleCount();
}
bool IAudioSource::open() {
    if(!isSequential()) setPos(0);
    m_err.clear();
    return true;
}
bool IAudioSource::close() {
    m_err.clear();
    return true;
}
QString IAudioSource::lastError() {
    return m_err;
}
void IAudioSource::setLastError(const QString &err) {
    m_err = err;
}
