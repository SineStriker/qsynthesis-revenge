//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_MEMORYAUDIOSOURCE_P_H
#define CHORUSKIT_MEMORYAUDIOSOURCE_P_H

#include "MemoryAudioSource.h"
#include "PositionableAudioSource_p.h"

#include <QMutex>

class MemoryAudioSourcePrivate: public PositionableAudioSourcePrivate {
    Q_DECLARE_PUBLIC(MemoryAudioSource)
public:
    IAudioSampleProvider *buffer;
    QMutex mutex;
    bool takeOwnership;
};

#endif // CHORUSKIT_MEMORYAUDIOSOURCE_P_H
