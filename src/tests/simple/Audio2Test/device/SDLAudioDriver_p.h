//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODRIVER_P_H
#define CHORUSKIT_SDLAUDIODRIVER_P_H

#include "SDLAudioDriver.h"
#include "AudioDriver_p.h"

class SDLAudioDriverPrivate: public AudioDriverPrivate {
    Q_DECLARE_PUBLIC(SDLAudioDriver)
public:
    int driverIndex;
};

#endif // CHORUSKIT_SDLAUDIODRIVER_P_H
