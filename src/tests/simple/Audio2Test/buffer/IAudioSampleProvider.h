//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_IAUDIOSAMPLEPROVIDER_H
#define CHORUSKIT_IAUDIOSAMPLEPROVIDER_H

#include <qglobal.h>

/**
 * @brief Base class for object containing audio data for read
 */
class IAudioSampleProvider {
public:
    virtual ~IAudioSampleProvider() = default;

    /**
     * Gets the sample at a specified channel and position.
     *
     * Note that if the sample data is stored continuously, performance will be better when using the pointer to read directly.
     *
     * @see isContinuous(), readPointerTo()
     */
    virtual float constSampleAt(int channel, qint64 pos) const = 0;

    /**
     * Gets the number of channels.
     */
    virtual int channelCount() const = 0;

    /**
     * Gets the number of samples that every channel contains.
     */
    virtual qint64 sampleCount() const = 0;

    /**
     * Gets whether the sample data is stored continuously or not. If so, the pointer can be used to read data directly.
     *
     * @see readPointerTo()
     */
    virtual bool isContinuous() const;

    /**
     * Gets the read pointer to a specified position of a specified channel.
     *
     * Note that his function should return @c nullptr if the sample data is not stored continuously.
     *
     * @see isContinuous
     */
    virtual const float *readPointerTo(int channel, qint64 startPos) const;

    /**
     * Gets the highest absolute sample value within a range of a specified channel.
     */
    float magnitude(int channel, qint64 startPos, qint64 length) const;

    /**
     * Gets the highest absolute sample value within a specified channel.
     */
    float magnitude(int channel) const;

    /**
     * Calculates the root mean squared sample value within a range of a specified channel.
     */
    float rms(int channel, qint64 startPos, qint64 length) const;

    /**
     * Calculates the root mean squared sample value within a specified channel.
     */
    float rms(int channel) const;
};



#endif // CHORUSKIT_IAUDIOSAMPLEPROVIDER_H
