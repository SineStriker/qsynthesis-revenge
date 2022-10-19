#include "IAudioPlayback_p.h"

IAudioPlaybackPrivate::IAudioPlaybackPrivate() {
}

IAudioPlaybackPrivate::~IAudioPlaybackPrivate() {
}

void IAudioPlaybackPrivate::init() {
    decoder = nullptr;
    state = IAudioPlayback::Stopped;
}

void IAudioPlaybackPrivate::setup() {
}

void IAudioPlaybackPrivate::dispose() {
}

void IAudioPlaybackPrivate::play() {
}

void IAudioPlaybackPrivate::stop() {
}
