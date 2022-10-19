#include "SDLPlayback.h"
#include "private/SDLPlayback_p.h"

SDLPlayback::SDLPlayback(QObject *parent) : SDLPlayback(*new SDLPlaybackPrivate(), parent) {
}

SDLPlayback::~SDLPlayback() {
}

SDLPlayback::SDLPlayback(SDLPlaybackPrivate &d, QObject *parent) : IAudioPlayback(d, parent) {
    d.init();
}
