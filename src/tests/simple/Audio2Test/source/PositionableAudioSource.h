//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_POSITIONABLEAUDIOSOURCE_H
#define CHORUSKIT_POSITIONABLEAUDIOSOURCE_H

#include "AudioSource.h"

class PositionableAudioSourcePrivate;

class PositionableAudioSource: public AudioSource {
    Q_DECLARE_PRIVATE(PositionableAudioSource)
public:
    PositionableAudioSource();
    virtual int length() const = 0;
    virtual int nextReadPosition() const;
    virtual void setNextReadPosition(int pos);
protected:
    PositionableAudioSource(PositionableAudioSourcePrivate &d);
};



#endif // CHORUSKIT_POSITIONABLEAUDIOSOURCE_H
