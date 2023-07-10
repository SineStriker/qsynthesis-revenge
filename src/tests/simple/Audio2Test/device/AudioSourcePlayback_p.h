//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIOSOURCEPLAYBACK_P_H
#define CHORUSKIT_AUDIOSOURCEPLAYBACK_P_H

#include "AudioSourcePlayback.h"

class AudioSourcePlaybackPrivate {
    Q_DECLARE_PUBLIC(AudioSourcePlayback)
public:
    AudioSourcePlayback *q_ptr;
    AudioSource *src;
    bool takeOwnership;
};

#endif // CHORUSKIT_AUDIOSOURCEPLAYBACK_P_H
