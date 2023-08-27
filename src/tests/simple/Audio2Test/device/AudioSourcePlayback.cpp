//
// Created by Crs_1 on 2023/7/10.
//

#include "AudioSourcePlayback.h"
#include "AudioSourcePlayback_p.h"

#include "AudioDevice.h"
#include "source/AudioSource.h"

AudioSourcePlayback::AudioSourcePlayback(AudioSource *src, bool takeOwnership): AudioSourcePlayback(*new AudioSourcePlaybackPrivate) {
    Q_D(AudioSourcePlayback);
    d->src = src;
    d->takeOwnership = takeOwnership;
}
AudioSourcePlayback::AudioSourcePlayback(AudioSourcePlaybackPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
AudioSourcePlayback::~AudioSourcePlayback() {
    Q_D(AudioSourcePlayback);
    if(d->takeOwnership) {
        delete d->src;
    }
}

AudioSource *AudioSourcePlayback::source() const {
    Q_D(const AudioSourcePlayback);
    return d->src;
}
void AudioSourcePlayback::setSource(AudioSource *src, bool takeOwnership) {
    Q_D(AudioSourcePlayback);
    d->src = src;
    d->takeOwnership = takeOwnership;
}

void AudioSourcePlayback::deviceWillStartCallback(AudioDevice *device) {
    Q_D(AudioSourcePlayback);
    d->src->open(device->bufferSize(), device->sampleRate());
}
void AudioSourcePlayback::deviceStoppedCallback() {
    Q_D(AudioSourcePlayback);
    d->src->close();
}

void AudioSourcePlayback::workCallback(const AudioSourceReadData &readData) {
    Q_D(AudioSourcePlayback);
    d->src->read(readData);
}
