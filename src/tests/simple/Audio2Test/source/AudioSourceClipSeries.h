//
// Created by Crs_1 on 2023/7/13.
//

#ifndef CHORUSKIT_AUDIOSOURCECLIPSERIES_H
#define CHORUSKIT_AUDIOSOURCECLIPSERIES_H

#include "PositionableAudioSource.h"
#include "utils/AudioClipBase.h"

typedef AudioClipBase<PositionableAudioSource> AudioSourceClip;

class AudioSourceClipSeriesPrivate;

class AudioSourceClipSeries: public PositionableAudioSource, public AudioClipSeriesBase<PositionableAudioSource> {
    Q_DECLARE_PRIVATE(AudioSourceClipSeries)
public:
    AudioSourceClipSeries();
    ~AudioSourceClipSeries() override;
    qint64 read(const AudioSourceReadData &readData) override;
    qint64 length() const override;
    qint64 nextReadPosition() const override;
    void setNextReadPosition(qint64 pos) override;
    bool open(qint64 bufferSize, double sampleRate) override;
    void close() override;
    bool addClip(const AudioSourceClip &clip) override;
    virtual bool removeClipAt(qint64 pos) override;
    void clearClips() override;
protected:
    AudioSourceClipSeries(AudioSourceClipSeriesPrivate &d);
};



#endif // CHORUSKIT_AUDIOSOURCECLIPSERIES_H
