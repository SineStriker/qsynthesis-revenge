//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_P_H
#define CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_P_H

#include "PositionableMixerAudioSource.h"
#include "MixerAudioSource_p.h"
#include "PositionableAudioSource_p.h"

class PositionableMixerAudioSourcePrivate: public PositionableAudioSourcePrivate, public IMixer<PositionableAudioSource> {
    Q_DECLARE_PUBLIC(PositionableMixerAudioSource)
public:
    void setNextReadPositionToAll(qint64 pos);
};

#endif // CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_P_H
