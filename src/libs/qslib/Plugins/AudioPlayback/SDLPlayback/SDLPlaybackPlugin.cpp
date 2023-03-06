#include "SDLPlaybackPlugin.h"

#include "SDLPlayback.h"

SDLPlaybackPlugin::SDLPlaybackPlugin(QObject *parent) : IAudioPlaybackPlugin(parent) {
}

SDLPlaybackPlugin::~SDLPlaybackPlugin() {
}

IAudioPlayback *SDLPlaybackPlugin::create(const QString &key, QObject *parent) {
    Q_UNUSED(key)
    return new SDLPlayback(parent);
}
