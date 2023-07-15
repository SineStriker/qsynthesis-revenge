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

class SDLEventPoller: public QObject {
    Q_OBJECT
    QAtomicInteger<bool> stopRequested = false;
public slots:
    void start();
    void quit();

signals:
    void event(QByteArray sdlEventData);

};

class SDLAudioDriverPrivate: public AudioDriverPrivate {
    Q_DECLARE_PUBLIC(SDLAudioDriver)
public:
    int driverIndex;
    QMutex mutex;
    SDLEventPoller eventPoller;
    QThread eventPollerThread;
    QMap<quint32, SDLAudioDevice *> openedDevices;

    void handleSDLEvent(const QByteArray &sdlEventData);
    void handleDeviceRemoved(quint32 devId);
};

#endif // CHORUSKIT_SDLAUDIODRIVER_P_H
