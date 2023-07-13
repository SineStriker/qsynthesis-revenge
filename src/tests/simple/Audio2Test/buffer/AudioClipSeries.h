//
// Created by Crs_1 on 2023/7/13.
//

#ifndef CHORUSKIT_AUDIOCLIPSERIES_H
#define CHORUSKIT_AUDIOCLIPSERIES_H

#include "utils/AudioClipBase.h"
#include "IAudioSampleProvider.h"

#include <set>

#include <QScopedPointer>

class AudioClipSeriesPrivate;

typedef AudioClipBase<IAudioSampleProvider> AudioClip;

class AudioClipSeries: public IAudioSampleProvider, public AudioClipSeriesBase<IAudioSampleProvider> {
    Q_DECLARE_PRIVATE(AudioClipSeries)
public:
    AudioClipSeries();
    ~AudioClipSeries();

    float constSampleAt(int channel, qint64 pos) const override;

    int channelCount() const override;
    void setChannelCount(int channelCount);

    qint64 sampleCount() const override;
    void setSampleCount(qint64 sampleCount);

    bool addClip(const AudioClip &clip) override;

protected:
    explicit AudioClipSeries(AudioClipSeriesPrivate &d);
    QScopedPointer<AudioClipSeriesPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIOCLIPSERIES_H
