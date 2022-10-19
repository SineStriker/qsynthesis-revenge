#ifndef SDLPLAYBACKPRIVATE_H
#define SDLPLAYBACKPRIVATE_H

#include "Api/private/IAudioPlayback_p.h"

#include "../SDLPlayback.h"

class SDLPlaybackPrivate : public IAudioPlaybackPrivate {
    Q_DECLARE_PUBLIC(SDLPlayback)
public:
    SDLPlaybackPrivate();
    ~SDLPlaybackPrivate();

    void init();

    void setup() override;
    void dispose() override;

    void play() override;
    void stop() override;
};

#endif // SDLPLAYBACKPRIVATE_H
