//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODEVICE_P_H
#define CHORUSKIT_SDLAUDIODEVICE_P_H

#include <SDL2/SDL.h>

#include "AudioDevice_p.h"
#include "SDLAudioDevice.h"

class SDLAudioDevicePrivate: public AudioDevicePrivate {
    Q_DECLARE_PUBLIC(SDLAudioDevice)
public:
    quint32 devId = 0;
    AudioDeviceCallback *audioDeviceCallback = nullptr;
    SDL_AudioSpec spec = {};

    void sdlCallback(quint8 *rawBuf, int length);
};

#endif // CHORUSKIT_SDLAUDIODEVICE_P_H
