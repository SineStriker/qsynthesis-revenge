//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIOSOURCEPLAYBACK_H
#define CHORUSKIT_AUDIOSOURCEPLAYBACK_H

#include "AudioDeviceCallback.h"

class AudioSourcePlaybackPrivate;

class AudioSourcePlayback: public AudioDeviceCallback {
    Q_DECLARE_PRIVATE(AudioSourcePlayback)
public:
    explicit AudioSourcePlayback(AudioSource *src, bool takeOwnership = false);
    ~AudioSourcePlayback();

    AudioSource *source() const;
    void resetSource(AudioSource *src, bool takeOwnership);

    void deviceWillStart(AudioDevice *device) override;
    void deviceStopped() override;
    void callback(const AudioSourceReadData &readData) override;

protected:
    explicit AudioSourcePlayback(AudioSourcePlaybackPrivate &d);
    QScopedPointer<AudioSourcePlaybackPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIOSOURCEPLAYBACK_H
