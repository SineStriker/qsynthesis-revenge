//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_POSITIONABLEAUDIOSOURCE_P_H
#define CHORUSKIT_POSITIONABLEAUDIOSOURCE_P_H

#include "PositionableAudioSource.h"
#include "AudioSource_p.h"

class PositionableAudioSourcePrivate: public AudioSourcePrivate {
    Q_DECLARE_PUBLIC(PositionableAudioSource)
public:
    qint64 position = 0;
};

#endif // CHORUSKIT_POSITIONABLEAUDIOSOURCE_P_H
