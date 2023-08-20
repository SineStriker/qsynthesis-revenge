//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_IAUDIOSAMPLECONTAINER_H
#define CHORUSKIT_IAUDIOSAMPLECONTAINER_H



#include "IAudioSampleProvider.h"

/**
 * @brief Base class for object containing audio data for read and write
 */

class IAudioSampleContainer : public IAudioSampleProvider {
public:

    /**
     * Gets the reference to the sample at a specified channel and position.
     *
     * Note that if the sample data is stored continuously, performance will be better when using the pointer to write directly.
     *
     * @return a non-const reference for writing.
     *
     * @see isContinuous(), writePointerTo()
     */
    virtual float &sampleAt(int channel, qint64 pos) = 0;

    /**
     * Gets the write pointer to a specified position of a specified channel.
     *
     * Note that his function should return \c nullptr if the sample data is not stored continuously.
     *
     * @see isContinuous()
     **/
    virtual float *writePointerTo(int channel, qint64 startPos);


    /**
     * Copies samples from another object to this one.
     *
     * @param destChannel   the channel of this object to copy samples to
     * @param destStartPos  the start position within destination channel
     * @param length        the number of samples to copy
     * @param src           the source object to read from
     * @param srcChannel    the channel of the source object to copy samples from
     * @param srcStartPos   the start position within source channel
     */
    void setSampleRange(int destChannel, qint64 destStartPos, qint64 length, const IAudioSampleProvider &src, int srcChannel, qint64 srcStartPos);

    /**
     * Copies samples from another object to this one.
     *
     * This overloaded function will copy all channels and samples from another object to this one.
     *
     * Note that if the number of channels or the number of samples in the source object is less than that in this
     * object or vice versa, only the overlapping part will be processed.
     */
    void setSampleRange(const IAudioSampleProvider &src);

    /**
     * Adds samples from another object to this one.
     *
     * @param destChannel   the channel of this object to copy samples to
     * @param destStartPos  the start position within destination channel
     * @param length        the number of samples to copy
     * @param src           the source object to read from
     * @param srcChannel    the channel of the source object to copy samples from
     * @param srcStartPos   the start position within source channel
     * @param gain          an optional gain to apply to the source before added
     */
    void addSampleRange(int destChannel, qint64 destStartPos, qint64 length, const IAudioSampleProvider &src, int srcChannel, qint64 srcStartPos, float gain = 1);

    /**
     * Adds samples from another object to this one.
     *
     * This overloaded function will add all channels and samples from another object to this one.
     *
     * Note that if the number of channels or the number of samples in the source object is less than that in this
     * object or vice versa, only the overlapping part will be processed.
     */
    void addSampleRange(const IAudioSampleProvider &src, float gain = 1);

    /**
     * Applies gain to samples within a range of a specified channel.
     *
     * @param destChannel   the channel to apply gain to
     * @param destStartPos  the start position within destination channel
     * @param length        the number of samples to apply gain to
     * @param gain          the gain to be applied
     */
    void gainSampleRange(int destChannel, qint64 destStartPos, qint64 length, float gain);

    /**
     * Applies gain to all samples within a specified channel.
     */
    void gainSampleRange(int destChannel, float gain);

    /**
     * Applies gain to all samples in all channels.
     */
    void gainSampleRange(float gain);

    /**
     * Sets samples within a range of a specified channel to zero.
     *
     * @param destChannel   the channel to clear
     * @param destStartPos  the start position within destination channel
     * @param length        the number of samples to clear
     */
    void clear(int destChannel, qint64 destStartPos, qint64 length);

    /**
     * Sets all samples within a specified channel to zero.
     */
    void clear(int destChannel);

    /**
     * Sets all samples in all channels to zero.
     */
    void clear();
};



#endif // CHORUSKIT_IAUDIOSAMPLECONTAINER_H
