#include "IAudioPlayback.h"
#include "private/IAudioPlayback_p.h"

IAudioPlayback::IAudioPlayback(QObject *parent)
    : IAudioPlayback(*new IAudioPlaybackPrivate(), parent) {
}

IAudioPlayback::~IAudioPlayback() {
}

void IAudioPlayback::setup(IAudioDecoder *decoder) {
    Q_D(IAudioPlayback);

    stop();

    d->decoder = decoder;

    d->setup();
}

void IAudioPlayback::dispose() {
    Q_D(IAudioPlayback);

    stop();

    d->dispose();

    d->decoder = nullptr;
}

void IAudioPlayback::play() {
    Q_D(IAudioPlayback);
    if (d->state == Playing) {
        return;
    }
    d->play();
}

void IAudioPlayback::stop() {
    Q_D(IAudioPlayback);
    if (d->state != Playing) {
        return;
    }
    d->stop();
}

IAudioPlayback::PlaybackState IAudioPlayback::state() const {
    Q_D(const IAudioPlayback);
    return static_cast<IAudioPlayback::PlaybackState>(d->state.loadRelaxed());
}

IAudioPlayback::IAudioPlayback(IAudioPlaybackPrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
