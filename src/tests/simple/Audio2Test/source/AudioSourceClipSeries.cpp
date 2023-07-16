//
// Created by Crs_1 on 2023/7/13.
//

#include "AudioSourceClipSeries.h"
#include "AudioSourceClipSeries_p.h"

#include <QDebug>

AudioSourceClipSeries::AudioSourceClipSeries(): AudioSourceClipSeries(*new AudioSourceClipSeriesPrivate) {
}
AudioSourceClipSeries::AudioSourceClipSeries(AudioSourceClipSeriesPrivate &d): PositionableAudioSource(d) {
}

AudioSourceClipSeries::~AudioSourceClipSeries() {
    close();
}
qint64 AudioSourceClipSeries::read(const AudioSourceReadData &readData) {
    Q_D(AudioSourceClipSeries);
    bool overlapPrev = false;
    auto nextClipIt = m_clips.lower_bound({d->position});
    decltype(nextClipIt) prevClipIt;
    if(nextClipIt != m_clips.begin()) {
        prevClipIt = nextClipIt;
        prevClipIt--;
        if(prevClipIt->position + prevClipIt->length > d->position) overlapPrev = true;
    }
    int chCnt = readData.buffer->channelCount();
    for(int ch = 0; ch < chCnt; ch++) {
        readData.buffer->clear(ch, readData.startPos, readData.length);
    }
    if(overlapPrev) {
        if(prevClipIt->content->nextReadPosition() != prevClipIt->startPos + d->position - prevClipIt->position) {
            prevClipIt->content->setNextReadPosition(prevClipIt->startPos + d->position - prevClipIt->position);
        }
        prevClipIt->content->read({readData.buffer, readData.startPos, std::min(readData.length, prevClipIt->length - (d->position - prevClipIt->position))});
    }
    for(;nextClipIt != m_clips.end() && d->position + readData.length > nextClipIt->position && d->position + readData.length <= nextClipIt->position + nextClipIt->length; nextClipIt++) {
        if(nextClipIt->content->nextReadPosition() != nextClipIt->startPos) {
            nextClipIt->content->setNextReadPosition(nextClipIt->startPos);
        }
        nextClipIt->content->read({readData.buffer, readData.startPos + nextClipIt->position - d->position, std::min(nextClipIt->length, d->position + readData.length - nextClipIt->position)});
    }
    d->position += readData.length;
    return readData.length;
}
qint64 AudioSourceClipSeries::length() const {
    return std::numeric_limits<qint64>::max();
}
qint64 AudioSourceClipSeries::nextReadPosition() const {
    Q_D(const AudioSourceClipSeries);
    return d->position;
}
void AudioSourceClipSeries::setNextReadPosition(qint64 pos) {
    Q_D(AudioSourceClipSeries);
    QMutexLocker locker(&d->mutex);
    d->position = pos;
}
bool AudioSourceClipSeries::open(qint64 bufferSize, double sampleRate) {
    Q_D(AudioSourceClipSeries);
    QMutexLocker locker(&d->mutex);
    return std::all_of(m_clips.begin(), m_clips.end(), [=](const AudioSourceClip &clip){
        return clip.content->open(bufferSize, sampleRate);
    });
}
void AudioSourceClipSeries::close() {
    Q_D(AudioSourceClipSeries);
    QMutexLocker locker(&d->mutex);
    std::for_each(m_clips.begin(), m_clips.end(), [=](const AudioSourceClip &clip){
        return clip.content->close();
    });
}

bool AudioSourceClipSeries::addClip(const AudioSourceClip &clip) {
    Q_D(AudioSourceClipSeries);
    QMutexLocker locker(&d->mutex);
    if(isOpen()) {
        if(!clip.content->open(bufferSize(), sampleRate())) {
            return false;
        }
    }
    return AudioClipSeriesBase::addClip(clip);
}
bool AudioSourceClipSeries::removeClipAt(qint64 pos) {
    Q_D(AudioSourceClipSeries);
    QMutexLocker locker(&d->mutex);
    auto it = findClipIt(pos);
    if(it == m_clips.end()) return false;
    it->content->close();
    return true;
}
void AudioSourceClipSeries::clearClips() {
    Q_D(AudioSourceClipSeries);
    QMutexLocker locker(&d->mutex);
    for(const auto &clip: m_clips) clip.content->close();
    m_clips.clear();
}
