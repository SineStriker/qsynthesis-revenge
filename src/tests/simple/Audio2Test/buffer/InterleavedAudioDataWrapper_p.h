//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_P_H
#define CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_P_H

#include "InterleavedAudioDataWrapper.h"

class InterleavedAudioDataWrapperPrivate {
    Q_DECLARE_PUBLIC(InterleavedAudioDataWrapper)
public:
    InterleavedAudioDataWrapper *q_ptr;
    float *data;
    int channelCount;
    qint64 sampleCount;
};

#endif // CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_P_H
