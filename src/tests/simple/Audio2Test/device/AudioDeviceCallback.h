//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODEVICECALLBACK_H
#define CHORUSKIT_AUDIODEVICECALLBACK_H

#include <QString>

#include "source/AudioSource.h"

class AudioDevice;
class AudioBuffer;

/* abstract */ class AudioDeviceCallback {

public:
    virtual void deviceWillStartCallback(AudioDevice *device);
    virtual void deviceStoppedCallback();
    virtual void deviceErrorCallback(const QString &error);

    virtual void workCallback(const AudioSourceReadData &readData) = 0;

};



#endif // CHORUSKIT_AUDIODEVICECALLBACK_H
