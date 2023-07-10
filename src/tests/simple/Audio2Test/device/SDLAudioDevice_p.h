//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODEVICE_P_H
#define CHORUSKIT_SDLAUDIODEVICE_P_H

#include "SDLAudioDevice.h"
#include "AudioDevice_p.h"

class SDLAudioDevicePrivate: public AudioDevicePrivate {
    Q_DECLARE_PUBLIC(SDLAudioDevice)
public:
    int devId = 0;
};

#endif // CHORUSKIT_SDLAUDIODEVICE_P_H
