//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_POSITIONABLEAUDIOSOURCE_H
#define CHORUSKIT_POSITIONABLEAUDIOSOURCE_H

#include "AudioSource.h"

class PositionableAudioSourcePrivate;

/* abstract */ class PositionableAudioSource: public AudioSource {
    Q_DECLARE_PRIVATE(PositionableAudioSource)
public:
    PositionableAudioSource();
    virtual qint64 length() const = 0;
    virtual qint64 nextReadPosition() const;
    virtual void setNextReadPosition(qint64 pos);
protected:
    explicit PositionableAudioSource(PositionableAudioSourcePrivate &d);
};



#endif // CHORUSKIT_POSITIONABLEAUDIOSOURCE_H
