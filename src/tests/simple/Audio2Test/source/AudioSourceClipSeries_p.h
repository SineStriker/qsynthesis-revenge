//
// Created by Crs_1 on 2023/7/13.
//

#ifndef CHORUSKIT_AUDIOSOURCECLIPSERIES_P_H
#define CHORUSKIT_AUDIOSOURCECLIPSERIES_P_H

#include "AudioSourceClipSeries.h"
#include "PositionableAudioSource_p.h"
#include <QMutex>

class AudioSourceClipSeriesPrivate: public PositionableAudioSourcePrivate {
    Q_DECLARE_PUBLIC(AudioSourceClipSeries);
public:
    QMutex mutex;
};

#endif // CHORUSKIT_AUDIOSOURCECLIPSERIES_P_H
