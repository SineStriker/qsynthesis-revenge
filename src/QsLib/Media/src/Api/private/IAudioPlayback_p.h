#ifndef IAUDIOPLAYBACKPRIVATE_H
#define IAUDIOPLAYBACKPRIVATE_H

#include "../IAudioPlayback.h"

class QSMEDIA_API IAudioPlaybackPrivate {
    Q_DECLARE_PUBLIC(IAudioPlayback)
public:
    IAudioPlaybackPrivate();
    virtual ~IAudioPlaybackPrivate();

    void init();

    virtual void setup();
    virtual void dispose();

    virtual void play();
    virtual void stop();

    IAudioPlayback *q_ptr;

    IAudioPlayback::PlaybackState state;

    IAudioDecoder *decoder;
};

#endif // IAUDIOPLAYBACKPRIVATE_H
