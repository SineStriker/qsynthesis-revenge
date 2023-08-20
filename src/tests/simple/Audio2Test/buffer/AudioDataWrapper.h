//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_AUDIODATAWRAPPER_H
#define CHORUSKIT_AUDIODATAWRAPPER_H

#include <QScopedPointer>

#include "IAudioSampleContainer.h"

class AudioDataWrapperPrivate;

/**
 * @brief A Wrapper that points to a set of pre-allocated arrays of sample data
 *
 * This class is useful when processing audio data stored planarly in the memory.
 *
 * @see InterLeavedAudioDataWrapper
 */
class AudioDataWrapper: public IAudioSampleContainer {
    Q_DECLARE_PRIVATE(AudioDataWrapper)
public:

    /**
     * Creates a wrapper that points to a set of pre-allocated arrays of sample data.
     *
     * @param data          a two-dimensional float array contains sample data
     * @param channelCount  the number of channels, indicating the number the first dimension that will be used
     * @param sampleCount   the number of samples, indicating the number of the second dimension that will be used
     * @param startPos      the optional start position of samples in each channel, indicating the starting index of the second dimension
     */
    AudioDataWrapper(float *const *data, int channelCount, qint64 sampleCount, qint64 startPos = 0);

    float &sampleAt(int channel, qint64 pos) override;
    float constSampleAt(int channel, qint64 pos) const override;

    int channelCount() const override;
    qint64 sampleCount() const override;

    float *writePointerTo(int channel, qint64 startPos) override;
    const float *readPointerTo(int channel, qint64 startPos) const override;

    /**
     * This is an overriden function.
     *
     * @return @c true always
     */
    bool isContinuous() const override;

    /**
     * Gets the data pointer that a specified channel points to.
     */
    float *data(int channel) const;

    /**
     * Resets the data. This function behaviors like the constructor, but it does that dynamically.
     *
     * @see AudioDataWrapper()
     */
    void reset(float *const *data, int channelCount, qint64 sampleCount, qint64 startPos = 0);

protected:
    QScopedPointer<AudioDataWrapperPrivate> d_ptr;
    AudioDataWrapper(AudioDataWrapperPrivate &d);
};



#endif // CHORUSKIT_AUDIODATAWRAPPER_H
