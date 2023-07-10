//
// Created by Crs_1 on 2023/7/11.
//

#include "SDLAudioDriver.h"
#include "SDLAudioDriver_p.h"

#include "SDLAudioDevice.h"

#include <SDL2/SDL.h>

SDLAudioDriver::SDLAudioDriver(QObject *parent): SDLAudioDriver(*new SDLAudioDriverPrivate, parent) {
}
SDLAudioDriver::SDLAudioDriver(SDLAudioDriverPrivate &d, QObject *parent): AudioDriver(d, parent) {
}

bool SDLAudioDriver::initialize() {
    if(SDL_AudioInit(name().toLocal8Bit()) == 0) {
        return AudioDriver::initialize();
    } else {
        setError(SDL_GetError());
        return false
    }
}
void SDLAudioDriver::finalize() {
    SDL_AudioQuit();
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
AudioDevice *SDLAudioDriver::createDevice(const QString &name, QObject *parent) {
    auto dev = new SDLAudioDevice(parent);
    dev->setName(name);
    dev->setDriver(this);
    return dev;
}
