//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_AUDIOSOURCE_P_H
#define CHORUSKIT_AUDIOSOURCE_P_H

#include "AudioSource.h"

class AudioSourcePrivate {
    Q_DECLARE_PUBLIC(AudioSource)
public:
    AudioSource *q_ptr;
};

#endif // CHORUSKIT_AUDIOSOURCE_P_H
