//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_SINEWAVEAUDIOSOURCE_H
#define CHORUSKIT_SINEWAVEAUDIOSOURCE_H

#include "PositionableAudioSource.h"

class SineWaveAudioSourcePrivate;

class SineWaveAudioSource: public PositionableAudioSource {
    Q_DECLARE_PRIVATE(SineWaveAudioSource)
public:
    SineWaveAudioSource();
    explicit SineWaveAudioSource(double frequency);
    ~SineWaveAudioSource() override = default;
    bool open(qint64 bufferSize, double sampleRate) override;
    qint64 read(const AudioSourceReadData &readData) override;
    qint64 length() const override;

    void setFrequency(double frequency);
    double frequency() const;
protected:
    explicit SineWaveAudioSource(SineWaveAudioSourcePrivate &d);
};



#endif // CHORUSKIT_SINEWAVEAUDIOSOURCE_H
