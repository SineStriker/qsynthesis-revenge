//
// Created by Crs_1 on 2023/7/11.
//

#include "SDLAudioDevice.h"
#include "SDLAudioDevice_p.h"

#include <SDL2/SDL.h>

static const QList<double> COMMON_SAMPLE_RATES = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000, 128000, 176400, 192000, 256000, 352800, 384000};

SDLAudioDevice::SDLAudioDevice(QObject *parent) : SDLAudioDevice(*new SDLAudioDevicePrivate, parent) {
    Q_D(SDLAudioDevice);
    auto devName = name().toUtf8().data();
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
        setError(SDL_GetError());
        return;
    }
    setChannelCount(preferredSpec.channels);
    setPreferredBufferSize(preferredSpec.samples);
    //TODO buffer size list;
    setAvailableBufferSizes({preferredSpec.samples});
    setPreferredSampleRate(preferredSpec.freq);
    //TODO sample rate list;
    setAvailableSampleRates(COMMON_SAMPLE_RATES);
}
SDLAudioDevice::SDLAudioDevice(SDLAudioDevicePrivate &d, QObject *parent): AudioDevice(d, parent) {
}
SDLAudioDevice::~SDLAudioDevice() {
}

bool SDLAudioDevice::start(AudioDeviceCallback *audioDeviceCallback) {
    return AudioDevice::start(audioDeviceCallback);
}
void SDLAudioDevice::stop() {
    AudioDevice::stop();
}
bool SDLAudioDevice::open(qint64 bufferSize, double sampleRate) {
    return IAudioStream::open(bufferSize, sampleRate);
}
void SDLAudioDevice::close() {
    IAudioStream::close();
}
