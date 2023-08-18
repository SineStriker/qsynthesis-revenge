//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODEVICE_H
#define CHORUSKIT_SDLAUDIODEVICE_H

#include "AudioDevice.h"

class SDLAudioDevicePrivate;
class SDLAudioDriver;

class SDLAudioDevice: public AudioDevice {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SDLAudioDevice)
    friend class SDLAudioDriver;
public:
    ~SDLAudioDevice() override;
    bool start(AudioDeviceCallback *audioDeviceCallback) override;
    void stop() override;
    bool open(qint64 bufferSize, double sampleRate) override;
    void close() override;
    void lock() override;
    void unlock() override;
    bool openControlPanel() override;

protected:
    SDLAudioDevice(const QString &name, SDLAudioDriver *driver);
    SDLAudioDevice(SDLAudioDevicePrivate &d, QObject *parent);

};



#endif // CHORUSKIT_SDLAUDIODEVICE_H
