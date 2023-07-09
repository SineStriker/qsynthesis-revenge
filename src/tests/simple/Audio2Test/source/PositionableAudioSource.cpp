//
// Created by Crs_1 on 2023/7/8.
//

#include "PositionableAudioSource.h"
#include "PositionableAudioSource_p.h"

PositionableAudioSource::PositionableAudioSource(): PositionableAudioSource(*new PositionableAudioSourcePrivate) {
}
PositionableAudioSource::PositionableAudioSource(PositionableAudioSourcePrivate &d): AudioSource(d) {
}

qint64 PositionableAudioSource::nextReadPosition() const {
    Q_D(const PositionableAudioSource);
    return d->position;
}
void PositionableAudioSource::setNextReadPosition(qint64 pos) {
    Q_D(PositionableAudioSource);
    d->position = pos;
}
