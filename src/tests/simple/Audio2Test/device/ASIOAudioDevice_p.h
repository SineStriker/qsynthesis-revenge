//
// Created by Crs_1 on 2023/8/17.
//

#ifndef CHORUSKIT_ASIOAUDIODEVICE_P_H
#define CHORUSKIT_ASIOAUDIODEVICE_P_H

#ifdef USE_FEATURE_ASIO

#include "ASIOAudioDevice.h"
#include "AudioDevice_p.h"

#include <qt_windows.h>
#include <combaseapi.h>
#include <iasiodrv.h>
#include <QMutex>

class ASIOAudioDevicePrivate: public AudioDevicePrivate {
    Q_DECLARE_PUBLIC(ASIOAudioDevice);
    IASIO *iasio;
    bool postOutput = false;
    QVector<ASIOBufferInfo> bufferInfoList;
    QVector<ASIOChannelInfo> channelInfoList;
    char errorMessageBuffer[128];

    static void bufferSwitch(long index, ASIOBool processNow);
    static void sampleRateDidChange(ASIOSampleRate sRate);
    static long asioMessage(long selector, long value, void* message, double* opt);
    static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);

    ASIOCallbacks callbacks = {
        &bufferSwitch,
        &sampleRateDidChange,
        &asioMessage,
        &bufferSwitchTimeInfo,
    };

    QMutex mutex;
};

#endif // USE_FEATURE_ASIO

#endif // CHORUSKIT_ASIOAUDIODEVICE_P_H
