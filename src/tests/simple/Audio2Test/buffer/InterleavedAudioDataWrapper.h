//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_H
#define CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_H

#include <QScopedPointer>

#include "IAudioSampleContainer.h"

class InterleavedAudioDataWrapperPrivate;

class InterleavedAudioDataWrapper: public IAudioSampleContainer {
    Q_DECLARE_PRIVATE(InterleavedAudioDataWrapper)
public:
    InterleavedAudioDataWrapper(float *data, int channelCount, qint64 sampleCount);
    ~InterleavedAudioDataWrapper();

    float &sampleAt(int channel, qint64 pos) override;
    float constSampleAt(int channel, qint64 pos) const override;
    int channelCount() const override;
    qint64 sampleCount() const override;

    float *data() const;
    void resetData(float *data, int channelCount, qint64 sampleCount);
protected:
    QScopedPointer<InterleavedAudioDataWrapperPrivate> d_ptr;
    InterleavedAudioDataWrapper(InterleavedAudioDataWrapperPrivate &d);
};



#endif // CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_H
