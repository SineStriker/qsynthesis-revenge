//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODEVICE_P_H
#define CHORUSKIT_AUDIODEVICE_P_H

#include "AudioDevice.h"

class AudioDevicePrivate {
    Q_DECLARE_PUBLIC(AudioDevice)
public:
    AudioDevice *q_ptr;
    AudioDriver *driver;
    int channelCount = 0;
    int activeChannelCount = -1;
    QList<qint64> availableBufferSizes;
    qint64 preferredBufferSize = 0;
    QList<double> availableSampleRates;
    double preferredSampleRate = 0;
    bool isStarted = false;
    bool isInitialized = false;
};

#endif // CHORUSKIT_AUDIODEVICE_P_H
