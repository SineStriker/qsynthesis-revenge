//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_AUDIODATAWRAPPER_P_H
#define CHORUSKIT_AUDIODATAWRAPPER_P_H

#include "AudioDataWrapper.h"

class AudioDataWrapperPrivate {
    Q_DECLARE_PUBLIC(AudioDataWrapper)
public:
    AudioDataWrapper *q_ptr;
    float *const *data;
    int channelCount;
    qint64 sampleCount;
    qint64 startPos;

};

#endif // CHORUSKIT_AUDIODATAWRAPPER_P_H
