//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H
#define CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H

#include <QMChronMap.h>
#include <QMutex>

#include "PositionableAudioSource.h"

class PositionableMixerAudioSourcePrivate;

class PositionableMixerAudioSource: public PositionableAudioSource {
    Q_DECLARE_PRIVATE(PositionableMixerAudioSource)
public:
    PositionableMixerAudioSource();
    ~PositionableMixerAudioSource() override;
    bool start(int bufferSize, double sampleRate) override;
    int read(const AudioSourceReadData &readData) override;
    void stop() override;
    int length() const override;
    void setNextReadPosition(int pos) override;

    void addSource(PositionableAudioSource *src, bool takeOwnership = false);
    void removeSource(PositionableAudioSource *src);
    void removeAllSource();
    QList<PositionableAudioSource *> sources() const;
protected:
    PositionableMixerAudioSource(PositionableMixerAudioSourcePrivate &d);
};



#endif // CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H
