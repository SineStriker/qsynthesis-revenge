//
// Created by Crs_1 on 2023/8/17.
//

#ifndef CHORUSKIT_ASIOAUDIODEVICE_H
#define CHORUSKIT_ASIOAUDIODEVICE_H

#ifndef USE_FEATURE_ASIO
#   warning ASIO Feature is not enabled
#else

#include "AudioDevice.h"

class ASIOAudioDevicePrivate;
class ASIOAudioDriver;
struct IASIO;

class ASIOAudioDevice: public AudioDevice {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ASIOAudioDevice)
public:
    ~ASIOAudioDevice();
    bool start(AudioDeviceCallback *audioDeviceCallback) override;
    void stop() override;
    bool open(qint64 bufferSize, double sampleRate) override;
    void close() override;
    void lock() override;
    void unlock() override;
    bool openControlPanel() override;

protected:
    friend class ASIOAudioDriver;
    ASIOAudioDevice(const QString &name, IASIO *iasio, ASIOAudioDriver *driver);
    ASIOAudioDevice(ASIOAudioDevicePrivate &d, QObject *parent);
};

#endif // USE_FEATURE_ASIO

#endif // CHORUSKIT_ASIOAUDIODEVICE_H
