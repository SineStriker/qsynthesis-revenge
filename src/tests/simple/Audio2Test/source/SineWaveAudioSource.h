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
    bool start(int bufferSize, double sampleRate) override;
    int read(const AudioSourceReadData &readData) override;
    int length() const override;

    void setFrequency(double frequency);
    double frequency() const;
protected:
    SineWaveAudioSource(SineWaveAudioSourcePrivate &d);
};



#endif // CHORUSKIT_SINEWAVEAUDIOSOURCE_H
