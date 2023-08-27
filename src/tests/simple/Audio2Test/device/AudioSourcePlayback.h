//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIOSOURCEPLAYBACK_H
#define CHORUSKIT_AUDIOSOURCEPLAYBACK_H

#include <QScopedPointer>
#include "AudioDeviceCallback.h"

class AudioSourcePlaybackPrivate;

class AudioSource;

/**
 * @brief Callback functions that starts, stops and reads data from an AudioSource
 *
 * In practice, TransportAudioSource is usually used to control the transportation of audio.
 *
 * @see AudioSource, TransportAudioSource
 */
class AudioSourcePlayback: public AudioDeviceCallback {
    Q_DECLARE_PRIVATE(AudioSourcePlayback)
public:
    /**
     * Constructor.
     * @param src the AudioSource to process
     * @param takeOwnership If the ownership of the AudioSource object is taken, the object will be deleted on destruction.
     */
    explicit AudioSourcePlayback(AudioSource *src, bool takeOwnership = false);
    ~AudioSourcePlayback();

    /**
     * Gets the AudioSource object used.
     */
    AudioSource *source() const;

    /**
     * Change the AudioSource object used dynamically. The ownership of the previous object is no more taken.
     */
    void setSource(AudioSource *src, bool takeOwnership = false);

    void deviceWillStartCallback(AudioDevice *device) override;
    void deviceStoppedCallback() override;
    void workCallback(const AudioSourceReadData &readData) override;

protected:
    explicit AudioSourcePlayback(AudioSourcePlaybackPrivate &d);
    QScopedPointer<AudioSourcePlaybackPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIOSOURCEPLAYBACK_H
