//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_POSITIONABLEAUDIOSOURCE_H
#define CHORUSKIT_POSITIONABLEAUDIOSOURCE_H

#include "AudioSource.h"


class PositionableAudioSource: public AudioSource {
public:
    virtual int length() const = 0;
    virtual int nextReadPosition() const = 0;
    virtual void setNextReadPosition(int pos) = 0;
};



#endif // CHORUSKIT_POSITIONABLEAUDIOSOURCE_H
