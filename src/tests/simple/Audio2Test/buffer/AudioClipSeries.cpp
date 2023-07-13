//
// Created by Crs_1 on 2023/7/13.
//

#include "AudioClipSeries.h"
#include "utils/AudioClipBase.h"
#include "AudioClipSeries_p.h"

#include <QList>

AudioClipSeries::AudioClipSeries(): AudioClipSeries(*new AudioClipSeriesPrivate){
}
AudioClipSeries::~AudioClipSeries() {
}

static inline bool inRange(qint64 x, qint64 l, qint64 r) {
    return x >= l && x < r;
}

float AudioClipSeries::constSampleAt(int channel, qint64 pos) const {
    auto d = d_ptr.data();
    auto it = findClipIt(pos);
    if(it == m_clips.end()) {
        return 0;
    }
    if(inRange(channel, 0, it->content->channelCount()) && inRange(pos - it->position + it->startPos, 0, it->content->sampleCount())) {
        return it->content->constSampleAt(channel, pos - it->position + it->startPos);
    } else {
        return 0;
    }
}
int AudioClipSeries::channelCount() const {
    Q_D(const AudioClipSeries);
    return d->channelCount;
}
void AudioClipSeries::setChannelCount(int channelCount) {
    Q_D(AudioClipSeries);
    d->channelCount = channelCount;
}
qint64 AudioClipSeries::sampleCount() const {
    Q_D(const AudioClipSeries);
    return d->sampleCount;
}
void AudioClipSeries::setSampleCount(qint64 sampleCount) {
    Q_D(AudioClipSeries);
    d->sampleCount = sampleCount;
}
AudioClipSeries::AudioClipSeries(AudioClipSeriesPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
bool AudioClipSeries::addClip(const AudioClip &clip) {
    Q_D(AudioClipSeries);
    qint64 clipL = clip.position;
    qint64 clipR = clip.position + clip.length;
    if(!inRange(clipL, 0, d->sampleCount)) return false;
    if(!inRange(clipR, 0, d->sampleCount)) return false;
    if(AudioClipSeriesBase::addClip(clip)) {
        return true;
    }
    return false;
}
