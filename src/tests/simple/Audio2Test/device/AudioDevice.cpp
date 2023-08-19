//
// Created by Crs_1 on 2023/7/10.
//

#include "AudioDevice.h"
#include "AudioDevice_p.h"

AudioDevice::AudioDevice(QObject *parent) : AudioDevice(*new AudioDevicePrivate, parent) {
}
AudioDevice::~AudioDevice() {
}
AudioDevice::AudioDevice(AudioDevicePrivate &d, QObject *parent): d_ptr(&d) {
    d.q_ptr = this;
}

AudioDriver *AudioDevice::driver() const {
    Q_D(const AudioDevice);
    return d->driver;
}
int AudioDevice::channelCount() const {
    Q_D(const AudioDevice);
    return d->channelCount;
}
int AudioDevice::activeChannelCount() const {
    Q_D(const AudioDevice);
    return (d->activeChannelCount < 0 || d->activeChannelCount > d->channelCount) ? d->channelCount : d->activeChannelCount;
}
void AudioDevice::setActiveChannelCount(int num) {
    Q_D(AudioDevice);
    d->activeChannelCount = num;
}
QList<qint64> AudioDevice::availableBufferSizes() const {
    Q_D(const AudioDevice);
    return d->availableBufferSizes;
}
qint64 AudioDevice::preferredBufferSize() const {
    Q_D(const AudioDevice);
    return d->preferredBufferSize;
}
QList<double> AudioDevice::availableSampleRates() const {
    Q_D(const AudioDevice);
    return d->availableSampleRates;
}
double AudioDevice::preferredSampleRate() const {
    Q_D(const AudioDevice);
    return d->preferredSampleRate;
}
void AudioDevice::setDriver(AudioDriver *driver) {
    Q_D(AudioDevice);
    d->driver = driver;
}
void AudioDevice::setChannelCount(int channelCount) {
    Q_D(AudioDevice);
    d->channelCount = channelCount;
}
void AudioDevice::setAvailableBufferSizes(const QList<qint64> &bufferSizes) {
    Q_D(AudioDevice);
    d->availableBufferSizes = bufferSizes;
}
void AudioDevice::setPreferredBufferSize(qint64 bufferSize) {
    Q_D(AudioDevice);
    d->preferredBufferSize = bufferSize;
}
void AudioDevice::setAvailableSampleRates(const QList<double> &sampleRates) {
    Q_D(AudioDevice);
    d->availableSampleRates = sampleRates;
}
void AudioDevice::setPreferredSampleRate(double sampleRate) {
    Q_D(AudioDevice);
    d->preferredSampleRate = sampleRate;
}

bool AudioDevice::start(AudioDeviceCallback *audioDeviceCallback) {
    Q_D(AudioDevice);
    return d->isStarted = true;
}
bool AudioDevice::isStarted() const {
    Q_D(const AudioDevice);
    return d->isStarted;
}
void AudioDevice::stop() {
    Q_D(AudioDevice);
    d->isStarted = false;
}
void AudioDevice::lock() {
}
void AudioDevice::unlock() {
}

bool AudioDevice::isInitialized() const {
    Q_D(const AudioDevice);
    return d->isInitialized;
}
void AudioDevice::setIsInitialized(bool isInitialized) {
    Q_D(AudioDevice);
    d->isInitialized = isInitialized;
}

bool AudioDevice::openControlPanel() {
    return false;
}
void AudioDevice::close() {
    IAudioStream::close();
    emit closed();
}
