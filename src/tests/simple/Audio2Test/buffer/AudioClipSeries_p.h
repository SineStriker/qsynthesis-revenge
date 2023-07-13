//
// Created by Crs_1 on 2023/7/13.
//

#ifndef CHORUSKIT_AUDIOCLIPSERIES_P_H
#define CHORUSKIT_AUDIOCLIPSERIES_P_H

#include "AudioClipSeries.h"

#include <set>

class AudioClipSeriesPrivate {
    Q_DECLARE_PUBLIC(AudioClipSeries)
public:
    AudioClipSeries *q_ptr;
    int channelCount = 2;
    qint64 sampleCount = std::numeric_limits<qint64>::max();
};

#endif // CHORUSKIT_AUDIOCLIPSERIES_P_H
