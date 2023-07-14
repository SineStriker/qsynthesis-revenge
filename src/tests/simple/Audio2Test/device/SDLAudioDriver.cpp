//
// Created by Crs_1 on 2023/7/11.
//

#include "SDLAudioDriver.h"
#include "SDLAudioDriver_p.h"

#include "SDLAudioDevice.h"

#include <SDL2/SDL.h>

#include <QDebug>

SDLAudioDriver::SDLAudioDriver(QObject *parent): SDLAudioDriver(*new SDLAudioDriverPrivate, parent) {
    Q_D(SDLAudioDriver);
    d->eventPollerThread.d = d;
}
SDLAudioDriver::SDLAudioDriver(SDLAudioDriverPrivate &d, QObject *parent): AudioDriver(d, parent) {
}
SDLAudioDriver::~SDLAudioDriver() {
    Q_D(SDLAudioDriver);
    SDLAudioDriver::finalize();
}

bool SDLAudioDriver::initialize() {
    Q_D(SDLAudioDriver);
    if(SDL_Init(SDL_INIT_AUDIO) == 0 && SDL_AudioInit(name().toLocal8Bit()) == 0) {
        d->startEventPoller();
        return AudioDriver::initialize();
    } else {
        setErrorString(SDL_GetError());
        return false;
    }
}
void SDLAudioDriver::finalize() {
    Q_D(SDLAudioDriver);
    d->stopEventPoller();
    SDL_AudioQuit();
    SDL_Quit();
    AudioDriver::finalize();
}
QStringList SDLAudioDriver::devices() {
    QStringList list;
    int cnt = SDL_GetNumAudioDevices(0);
    for(int i = 0; i < cnt; i++) {
        list.append(QString::fromUtf8(SDL_GetAudioDeviceName(i, 0)));
    }
    return list;
}
QString SDLAudioDriver::defaultDevice() {
    char *name;
    SDL_AudioSpec spec;
    if(SDL_GetDefaultAudioInfo(&name, &spec, 0) == 0) {
        auto s = QString::fromUtf8(name);
        SDL_free(name);
        return s;
    } else {
        return {};
    }
}
AudioDevice *SDLAudioDriver::createDevice(const QString &name) {
    auto dev = new SDLAudioDevice(name, this, this);
    return dev;
}


void SDLEventPollerThread::run() {
    forever {
        if(quitFlag) break;
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0) {
            if(e.type == SDL_AUDIODEVICEADDED && e.adevice.iscapture == 0) {
                qDebug() << "SDL_AUDIODEVICEADDED";
                d->_q_deviceChanged();
            } else if(e.type == SDL_AUDIODEVICEREMOVED && e.adevice.iscapture == 0) {
                qDebug() << "SDL_AUDIODEVICEREMOVED";
                d->handleDeviceRemoved(e.adevice.which);
                d->_q_deviceChanged();
            }
        }
        SDL_Delay(1);
    }
}

void SDLAudioDriver::addOpenedDevice(quint32 devId, SDLAudioDevice *dev) {
    Q_D(SDLAudioDriver);
    QMutexLocker locker(&d->mutex);
    d->openedDevices.insert(devId, dev);
}

void SDLAudioDriver::removeOpenedDevice(quint32 devId) {
    Q_D(SDLAudioDriver);
    QMutexLocker locker(&d->mutex);
    d->openedDevices.remove(devId);
}
QList<SDLAudioDriver *> SDLAudioDriver::getDrivers() {
    int cnt = SDL_GetNumAudioDrivers();
    QList<SDLAudioDriver *> list;
    for(int i = 0; i < cnt; i++) {
        auto name = QString::fromLatin1(SDL_GetAudioDriver(i));
        if(name == "disk" || name == "dummy") continue;
        auto drv = new SDLAudioDriver;
        drv->setName(name);
        list.append(drv);
    }
    return list;
}


void SDLAudioDriverPrivate::startEventPoller() {
    eventPollerThread.start();
}
void SDLAudioDriverPrivate::stopEventPoller() {
    eventPollerThread.quitFlag = true;
    eventPollerThread.quit();
    eventPollerThread.wait();
}
void SDLAudioDriverPrivate::_q_deviceChanged() {
    Q_Q(SDLAudioDriver);
    emit q->deviceChanged();
}
void SDLAudioDriverPrivate::handleDeviceRemoved(quint32 devId) {
    QMutexLocker locker(&mutex);
    auto it = openedDevices.find(devId);
    if(it == openedDevices.end()) return;
    it.value()->close();
}
