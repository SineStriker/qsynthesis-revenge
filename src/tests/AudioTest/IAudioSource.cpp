//
// Created by Crs_1 on 2023/5/6.
//

#include "IAudioSource.h"


bool IAudioSource::setReadMode(IAudioSource::ReadMode mode) {
    return true;
}
IAudioSource::ReadMode IAudioSource::readMode() const {
    return IAudioSource::Immediate;
}
quint64 IAudioSource::pos() const {
    return 0;
}
bool IAudioSource::setPos(quint64 pos) {
    return false;
}
bool IAudioSource::setSampleRate(quint32 sampleRate) {
    return false;
}
bool IAudioSource::open() {
    if(!isSequential()) setPos(0);
    m_err.clear();
    return true;
}
void IAudioSource::close() {
    m_err.clear();
}
QString IAudioSource::lastError() const {
    return m_err;
}
void IAudioSource::setLastError(const QString &err) {
    m_err = err;
}
