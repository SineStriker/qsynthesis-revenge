#include "IAudioPlayback.h"
#include "private/IAudioPlayback_p.h"

IAudioPlayback::IAudioPlayback(QObject *parent)
    : IAudioPlayback(*new IAudioPlaybackPrivate(), parent) {
}

IAudioPlayback::~IAudioPlayback() {
    stop();
}

bool IAudioPlayback::setup(const PlaybackArguments &args) {
    Q_D(IAudioPlayback);

    stop();

    // Default Arguments
    d->bufferSamples = args.bufferSamples;
    d->sampleRate = args.sampleRate;
    d->channels = args.channels;

    // Setup
    if (d->setup(args.custom)) {
        return true;
    }

    return false;
}

void IAudioPlayback::dispose() {
    Q_D(IAudioPlayback);

    stop();

    d->dispose();
}

void IAudioPlayback::setDecoder(IAudioDecoder *decoder) {
    Q_D(IAudioPlayback);

    stop();

    d->decoder = decoder;
}

bool IAudioPlayback::isReady() const {
    Q_D(const IAudioPlayback);
    return d->decoder;
}

void IAudioPlayback::play() {
    Q_D(IAudioPlayback);
    if (!isReady()) {
        return;
    }
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
    return static_cast<IAudioPlayback::PlaybackState>(d->state.load());
}

QStringList IAudioPlayback::drivers() const {
    return {};
}

QString IAudioPlayback::currentDriver() const {
    return QString();
}

bool IAudioPlayback::setDriver(const QString &driver) {
    Q_UNUSED(driver);
    return false;
}

QStringList IAudioPlayback::devices() const {
    return {};
}

QString IAudioPlayback::currentDevice() const {
    return QString();
}

bool IAudioPlayback::setDevice(const QString &device) {
    Q_UNUSED(device);
    return false;
}

int IAudioPlayback::bufferSamples() const {
    Q_D(const IAudioPlayback);
    return d->bufferSamples;
}

int IAudioPlayback::sampleRate() const {
    Q_D(const IAudioPlayback);
    return d->sampleRate;
}

int IAudioPlayback::channels() const {
    Q_D(const IAudioPlayback);
    return d->channels;
}

IAudioPlayback::IAudioPlayback(IAudioPlaybackPrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
