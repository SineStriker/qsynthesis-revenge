//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODRIVER_P_H
#define CHORUSKIT_SDLAUDIODRIVER_P_H

#include "SDLAudioDriver.h"
#include "AudioDriver_p.h"

#include <QThread>
#include <QMap>
#include <QMutex>

class SDLEventPollerThread: public QThread {
public:
    [[noreturn]] void run() override;
    SDLAudioDriverPrivate *d;
    QAtomicInteger<bool> quitFlag = false;
};

class SDLAudioDriverPrivate: public AudioDriverPrivate {
    Q_DECLARE_PUBLIC(SDLAudioDriver)
public:
    int driverIndex;
    QMutex mutex;
    SDLEventPollerThread eventPollerThread;
    QMap<quint32, SDLAudioDevice *> openedDevices;
    void startEventPoller();
    void stopEventPoller();
    void _q_deviceChanged();
    void handleDeviceRemoved(quint32 devId);
};

#endif // CHORUSKIT_SDLAUDIODRIVER_P_H
