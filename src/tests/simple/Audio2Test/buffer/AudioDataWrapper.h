//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_AUDIODATAWRAPPER_H
#define CHORUSKIT_AUDIODATAWRAPPER_H

#include <QScopedPointer>

#include "IAudioSampleContainer.h"

class AudioDataWrapperPrivate;

class AudioDataWrapper: public IAudioSampleContainer {
    Q_DECLARE_PRIVATE(AudioDataWrapper)
public:
    AudioDataWrapper(float *const *data, int channelCount, qint64 sampleCount, qint64 startPos = 0);

    float &sampleAt(int channel, qint64 pos) override;
    float constSampleAt(int channel, qint64 pos) const override;
    int channelCount() const override;
    qint64 sampleCount() const override;

    float *writePointerTo(int channel, qint64 startPos) override;
    bool isContinuous() const override;
    const float *readPointerTo(int channel, qint64 startPos) const override;

    float *data(int channel) const;
    void resetData(float *const *data, int channelCount, qint64 sampleCount, qint64 startPos = 0);
protected:
    QScopedPointer<AudioDataWrapperPrivate> d_ptr;
    AudioDataWrapper(AudioDataWrapperPrivate &d);
};



#endif // CHORUSKIT_AUDIODATAWRAPPER_H
