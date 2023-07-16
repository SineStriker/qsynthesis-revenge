//
// Created by Crs_1 on 2023/7/16.
//

#ifndef CHORUSKIT_CHANNELROUTINGAUDIOSOURCE_P_H
#define CHORUSKIT_CHANNELROUTINGAUDIOSOURCE_P_H

#include "ChannelRoutingAudioSource.h"
#include "MixerAudioSource_p.h"

class ChannelRoutingAudioSourcePrivate: public AudioSourcePrivate, public IMixer<AudioSource> {
    Q_DECLARE_PUBLIC(ChannelRoutingAudioSource)
public:
    AudioBuffer tmpBuf;
};

#endif // CHORUSKIT_CHANNELROUTINGAUDIOSOURCE_P_H
