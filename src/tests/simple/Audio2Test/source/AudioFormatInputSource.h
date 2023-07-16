//
// Created by Crs_1 on 2023/7/12.
//

#ifndef CHORUSKIT_AUDIOFORMATINPUTSOURCE_H
#define CHORUSKIT_AUDIOFORMATINPUTSOURCE_H

#include "PositionableAudioSource.h"

class AudioFormatInputSourcePrivate;
class AudioFormatIO;

class AudioFormatInputSource: public PositionableAudioSource {
    Q_DECLARE_PRIVATE(AudioFormatInputSource);
public:
    enum ResampleMode {
        SincBestQuality = 0,
        SincMediumQuality,
        SincFastest,
        ZeroOrderHold,
        Linear,
    };
    explicit AudioFormatInputSource(AudioFormatIO *audioFormatIo = nullptr);
    ~AudioFormatInputSource();

    qint64 read(const AudioSourceReadData &readData) override;
    qint64 length() const override;
    void setNextReadPosition(qint64 pos) override;
    bool open(qint64 bufferSize, double sampleRate) override;
    void close() override;

    void setAudioFormatIo(AudioFormatIO *audioFormatIo);
    AudioFormatIO *audioFormatIo() const;

    void flush();

    void setDoStereoize(bool doStereoize);
    bool doStereoize() const;

    void setResamplerMode(ResampleMode mode);
    ResampleMode resampleMode() const;

protected:
    explicit AudioFormatInputSource(AudioFormatInputSourcePrivate &d);
};


#endif // CHORUSKIT_AUDIOFORMATINPUTSOURCE_H
