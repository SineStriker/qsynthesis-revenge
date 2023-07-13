//
// Created by Crs_1 on 2023/7/11.
//

#include "SDLAudioDevice.h"
#include "SDLAudioDevice_p.h"
#include "buffer/InterleavedAudioDataWrapper.h"

#include <SDL2/SDL.h>

#include <QDebug>

#include "SDLAudioDriver.h"

static const QList<double> COMMON_SAMPLE_RATES = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000, 128000, 176400, 192000, 256000, 352800, 384000};

static const QList<qint64> COMMON_SDL_BUFFER_SIZES = {512, 1024, 2048, 4096, 8192};

SDLAudioDevice::SDLAudioDevice(const QString &name, AudioDriver *driver, QObject *parent) : SDLAudioDevice(*new SDLAudioDevicePrivate, parent) {
    Q_D(SDLAudioDevice);
    setName(name);
    setDriver(driver);
    auto devName = name.toUtf8().data();
    SDL_AudioSpec preferredSpec;
    int cnt = SDL_GetNumAudioDevices(0);
    int devIndex = 0;
    for(int i = 0; i < cnt; i++) {
        if(std::strcmp(devName, SDL_GetAudioDeviceName(i, 0)) == 0) {
            devIndex = i;
            break;
        }
    }
    if(SDL_GetAudioDeviceSpec(devIndex, 0, &preferredSpec) != 0) {
        setErrorString(SDL_GetError());
        return;
    }
    setChannelCount(preferredSpec.channels);
    if(preferredSpec.samples == 0) preferredSpec.samples = 1024;
    setPreferredBufferSize(preferredSpec.samples);
    setAvailableBufferSizes(COMMON_SDL_BUFFER_SIZES);
    if(preferredSpec.freq == 0) preferredSpec.freq = 48000;
    setPreferredSampleRate(preferredSpec.freq);
    setAvailableSampleRates(COMMON_SAMPLE_RATES);
}
SDLAudioDevice::SDLAudioDevice(SDLAudioDevicePrivate &d, QObject *parent): AudioDevice(d, parent) {
}
SDLAudioDevice::~SDLAudioDevice() {
    close();
}

static void sdlCallback(void *d, quint8 *rawBuf, int length) {
    reinterpret_cast<SDLAudioDevicePrivate *>(d)->sdlCallback(rawBuf, length);
}

bool SDLAudioDevice::open(qint64 bufferSize, double sampleRate) {
    Q_D(SDLAudioDevice);
    if(isOpened()) close();
    d->spec = {
        .freq = (int)sampleRate,
        .format = AUDIO_F32SYS,
        .channels = (quint8)activeChannelCount(),
        .silence = 0,
        .samples = (quint16)bufferSize,
        .callback = sdlCallback,
        .userdata = d,
    };
    d->devId = SDL_OpenAudioDevice(name().toUtf8().data(), 0, &d->spec, nullptr, 0);
    if(d->devId == 0) {
        setErrorString(SDL_GetError());
        return false;
    }
    reinterpret_cast<SDLAudioDriver *>(d->driver)->addOpenedDevice(d->devId, this);
    return IAudioStream::open(bufferSize, sampleRate);
}
void SDLAudioDevice::close() {
    Q_D(SDLAudioDevice);
    SDL_CloseAudioDevice(d->devId);
    reinterpret_cast<SDLAudioDriver *>(d->driver)->removeOpenedDevice(d->devId);
    d->devId = 0;
    d->spec = {};
}

bool SDLAudioDevice::start(AudioDeviceCallback *audioDeviceCallback) {
    Q_D(SDLAudioDevice);
    d->audioDeviceCallback = audioDeviceCallback;
    audioDeviceCallback->deviceWillStartCallback(this);
    SDL_PauseAudioDevice(d->devId, 0);
    return AudioDevice::start(audioDeviceCallback);
}
void SDLAudioDevice::stop() {
    Q_D(SDLAudioDevice);
    SDL_PauseAudioDevice(d->devId, 1);
    d->audioDeviceCallback->deviceStoppedCallback();
    AudioDevice::stop();
}

void SDLAudioDevicePrivate::sdlCallback(quint8 *rawBuf, int length) {
    Q_Q(SDLAudioDevice);
    SDL_memset(rawBuf, 0, length);
    InterleavedAudioDataWrapper buf(reinterpret_cast<float *>(rawBuf), spec.channels, length / spec.channels / 4);
    audioDeviceCallback->workCallback(&buf);
}
