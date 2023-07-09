//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_SINEWAVEAUDIOSOURCE_P_H
#define CHORUSKIT_SINEWAVEAUDIOSOURCE_P_H

#include "SineWaveAudioSource.h"
#include "PositionableAudioSource_p.h"

#include <QMutex>

class SineWaveAudioSourcePrivate: public PositionableAudioSourcePrivate {
    Q_DECLARE_PUBLIC(SineWaveAudioSource);
public:
    double freq = 0;
    QMutex mutex;
};

#endif // CHORUSKIT_SINEWAVEAUDIOSOURCE_P_H
