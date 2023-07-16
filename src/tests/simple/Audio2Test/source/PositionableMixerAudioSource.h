//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H
#define CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H

#include <QMChronMap.h>
#include <QMutex>

#include "PositionableAudioSource.h"

class PositionableMixerAudioSourcePrivate;

class PositionableMixerAudioSource: public QObject, public PositionableAudioSource {
    Q_OBJECT
#define d_ptr PositionableAudioSource::d_ptr
    Q_DECLARE_PRIVATE(PositionableMixerAudioSource)
#undef d_ptr
public:
    PositionableMixerAudioSource();
    ~PositionableMixerAudioSource() override;
    bool open(qint64 bufferSize, double sampleRate) override;
    qint64 read(const AudioSourceReadData &readData) override;
    void close() override;
    qint64 length() const override;
    void setNextReadPosition(qint64 pos) override;

    bool addSource(PositionableAudioSource *src, bool takeOwnership = false);
    bool removeSource(PositionableAudioSource *src);
    void removeAllSource();
    QList<PositionableAudioSource *> sources() const;

    void setGain(float gain);
    float gain() const;

    void setPan(float pan);
    float pan() const;

signals:
    void meterUpdated(float magnitudeLeft, float magnitudeRight);

protected:
    explicit PositionableMixerAudioSource(PositionableMixerAudioSourcePrivate &d);
};



#endif // CHORUSKIT_POSITIONABLEMIXERAUDIOSOURCE_H
