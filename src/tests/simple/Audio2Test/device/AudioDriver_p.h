//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODRIVER_P_H
#define CHORUSKIT_AUDIODRIVER_P_H

#include "AudioDriver.h"

class AudioDriverPrivate {
    Q_DECLARE_PUBLIC(AudioDriver)
public:
    AudioDriver *q_ptr;
    bool isInitialized = false;
};

#endif // CHORUSKIT_AUDIODRIVER_P_H
