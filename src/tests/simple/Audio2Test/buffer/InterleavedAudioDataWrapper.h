//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_H
#define CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_H

#include <QScopedPointer>

#include "IAudioSampleContainer.h"

class InterleavedAudioDataWrapperPrivate;

/**
 * @brief A Wrapper that points to a pre-allocated array of interleaved audio sample data.
 * @see AudioDataWrapper
 */
class InterleavedAudioDataWrapper: public IAudioSampleContainer {
    Q_DECLARE_PRIVATE(InterleavedAudioDataWrapper)
public:
    /**
     * Creates a wrapper that points to a set of pre-allocated arrays of sample data.
     * @param data          a float array contains sample data
     * @param channelCount  the number of channels
     * @param sampleCount   the number of samples
     */
    InterleavedAudioDataWrapper(float *data, int channelCount, qint64 sampleCount);

    ~InterleavedAudioDataWrapper();

    float &sampleAt(int channel, qint64 pos) override;
    float constSampleAt(int channel, qint64 pos) const override;
    int channelCount() const override;
    qint64 sampleCount() const override;

    float *writePointerTo(int channel, qint64 startPos) override;
    const float *readPointerTo(int channel, qint64 startPos) const override;

    /**
     * This is an overriden function.
     * @return @c false, unless the number of channel is one.
     */
    bool isContinuous() const override;

    /**
     * Returns the array that this wrapper points to.
     */
    float *data() const;

    /**
     * Resets the data. This function behaviors like the constructor, but it does that dynamically.
     * @see InterleavedAudioDataWrapper()
     */
    void resetData(float *data, int channelCount, qint64 sampleCount);

protected:
    QScopedPointer<InterleavedAudioDataWrapperPrivate> d_ptr;
    InterleavedAudioDataWrapper(InterleavedAudioDataWrapperPrivate &d);
};



#endif // CHORUSKIT_INTERLEAVEDAUDIODATAWRAPPER_H
