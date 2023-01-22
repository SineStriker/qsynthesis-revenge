#include "IAudioPlayback_p.h"

IAudioPlaybackPrivate::IAudioPlaybackPrivate() {
}

IAudioPlaybackPrivate::~IAudioPlaybackPrivate() {
}

void IAudioPlaybackPrivate::init() {
    decoder = nullptr;
    available = false;
    state = IAudioPlayback::Stopped;

    bufferSamples = 0;
    sampleRate = 0;
    channels = 0;
}

bool IAudioPlaybackPrivate::setup(const QVariantMap &args) {
    Q_UNUSED(args);
    return false;
}

void IAudioPlaybackPrivate::dispose() {
}

void IAudioPlaybackPrivate::play() {
}

void IAudioPlaybackPrivate::stop() {
}
