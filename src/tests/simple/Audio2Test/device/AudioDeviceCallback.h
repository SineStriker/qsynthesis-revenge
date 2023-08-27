//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODEVICECALLBACK_H
#define CHORUSKIT_AUDIODEVICECALLBACK_H

class AudioDevice;
class AudioBuffer;
class AudioSourceReadData;

/**
 * @brief Callback functions of audio device
 * @see AudioDevice
 */
class AudioDeviceCallback {

public:
    /**
     * Called when device is about to start.
     * @param device the target audio device
     */
    virtual void deviceWillStartCallback(AudioDevice *device) = 0;

    /**
     * Called when device is stopped.
     */
    virtual void deviceStoppedCallback() = 0;

    /**
     * Called on each frame of audio streaming.
     */
    virtual void workCallback(const AudioSourceReadData &readData) = 0;

};



#endif // CHORUSKIT_AUDIODEVICECALLBACK_H
