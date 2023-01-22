#ifndef IAUDIOPLAYBACKPRIVATE_H
#define IAUDIOPLAYBACKPRIVATE_H

#include "../IAudioPlayback.h"

class QSMEDIA_API IAudioPlaybackPrivate {
    Q_DECLARE_PUBLIC(IAudioPlayback)
public:
    IAudioPlaybackPrivate();
    virtual ~IAudioPlaybackPrivate();

    void init();

    virtual bool setup(const QVariantMap &args);
    virtual void dispose();

    virtual void play();
    virtual void stop();

    IAudioPlayback *q_ptr;

    IAudioDecoder *decoder;
    bool available;

    // Atomic int as playback state
    volatile int state;

    // Arguments
    int bufferSamples;
    int sampleRate;
    int channels;
};

#endif // IAUDIOPLAYBACKPRIVATE_H
