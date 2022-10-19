#include "IAudioPlayback_p.h"

IAudioPlaybackPrivate::IAudioPlaybackPrivate() {
}

IAudioPlaybackPrivate::~IAudioPlaybackPrivate() {
}

void IAudioPlaybackPrivate::init() {
    state = IAudioPlayback::Stopped;
    decoder = nullptr;
}

void IAudioPlaybackPrivate::setup() {
}

void IAudioPlaybackPrivate::dispose() {
}

void IAudioPlaybackPrivate::play() {
}

void IAudioPlaybackPrivate::stop() {
}
