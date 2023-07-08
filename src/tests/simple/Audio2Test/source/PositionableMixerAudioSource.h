//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H
#define CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H

#include <QMChronMap.h>

#include "PositionableAudioSource.h"



class PositionableMixerAudioSource: public PositionableAudioSource {
public:
    ~PositionableMixerAudioSource() override;
    bool start(int bufferSize, double sampleRate) override;
    bool isStarted() const override;
    int read(const AudioSourceReadData &readData) override;
    void stop() override;
    int length() const override;
    int nextReadPosition() const override;
    void setNextReadPosition(int pos) override;

    void addSource(PositionableAudioSource *src, bool takeOwnership);
    void removeSource(PositionableAudioSource *src);
    void removeAllSource();
    QList<PositionableAudioSource *> sources() const;
private:
    QMChronMap<PositionableAudioSource *, bool> m_sourceDict;
    bool m_isStarted = false;
    int m_position = 0;
};



#endif // CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H
