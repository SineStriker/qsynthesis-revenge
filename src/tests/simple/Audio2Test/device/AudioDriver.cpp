//
// Created by Crs_1 on 2023/7/10.
//

#include "AudioDriver.h"
#include "AudioDriver_p.h"

AudioDriver::AudioDriver(QObject *parent): AudioDriver(*new AudioDriverPrivate, parent) {
}
AudioDriver::AudioDriver(AudioDriverPrivate &d, QObject *parent): QObject(parent), d_ptr(&d) {
    d.q_ptr = this;
}
AudioDriver::~AudioDriver() {
}
bool AudioDriver::initialize() {
    Q_D(AudioDriver);
    return d->isInitialized = true;
}
void AudioDriver::finalize() {
    Q_D(AudioDriver);
    d->isInitialized = false;
}
bool AudioDriver::isInitialized() const {
    Q_D(const AudioDriver);
    return d->isInitialized;
}
QString AudioDriver::defaultDevice() const {
    return QString();
}
