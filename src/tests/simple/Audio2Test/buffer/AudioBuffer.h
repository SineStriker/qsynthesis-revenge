#ifndef CHORUSKIT_AUDIOBUFFER_H
#define CHORUSKIT_AUDIOBUFFER_H

#include "IAudioSampleContainer.h"
#include <QVector>

/**
 * @brief A container of audio sample data
 *
 * AudioBuffer use a two-dimensional QVector to store multi-channel audio internally and encapsulates some interfaces to access the internal vector.
 */
class AudioBuffer: public IAudioSampleContainer {

public:

    /**
     * Creates an empty buffer with zero channel and zero sample initially.
     */
    AudioBuffer();

    /**
     * Creates an empty buffer with specified number of channels and samples.
     */
    AudioBuffer(int channelCount, qint64 sampleCount);

    float &sampleAt(int channel, qint64 pos) override;
    float constSampleAt(int channel, qint64 pos) const override;

    float *writePointerTo(int channel, qint64 startPos) override;
    const float *readPointerTo(int channel, qint64 startPos) const override;

    int channelCount() const override;
    qint64 sampleCount() const override;

    /**
     * This is an overriden function.
     *
     * @return @c true always
     */
    bool isContinuous() const override;

    /**
     * Changes the size of this buffer.
     *
     * For both expanding and contracting, this will keep existing data. New samples will be set to zero after expanding.
     *
     * @param newChannelCount   the optional new number of channels
     * @param newSampleCount    the optional new number of samples
     */
    void resize(int newChannelCount = -1, qint64 newSampleCount = -1);

    /**
     * Gets the pointer to the data of a specified channel for writing. This function behaviors the same as @c writePointerTo(channel,0).
     *
     * Note that only until this buffer is resized does the pointer remains valid.
     *
     * @see resize(), writePointerTo()
     */
    float *data(int channel);

    /**
     * Gets the pointer to the data of a specified channel for reading. This function behaviors the same as @c readPointerTo(channel,0).
     *
     * Note that only until this buffer is resized does the pointer remains valid.
     *
     * @see resize(), readPointerTo()
     */
    float const *constData(int channel) const;

    /**
     * Gets the reference to the QVector that stores the sample data with a specified channel.
     *
     * Note that only until this buffer is resized does the reference remains valid.
     *
     * @see resize()
     */
    QVector<float> &vector(int channel);

    /**
     * Gets the const reference to the QVector that stores the sample data with a specified channel.
     *
     * Note that only until this buffer is resized does the reference remains valid.
     *
     * @see resize()
     */
    const QVector<float> &constVector(int channel) const;

    /**
     * Creates a sub-buffer that contains samples of ranges within specified channels.
     *
     * @param startChannelIndex the index of channel where the section starts at
     * @param startSampleCount  the position of sample where the section starts at
     * @param channelSize       the optional number of channels in the section (all channels by default)
     * @param length            the optional number of samples in the section (all samples by default)
     *
     * @return the new AudioBuffer
     */
    AudioBuffer slice(int startChannelIndex, qint64 startSampleCount, int channelSize = -1, qint64 length = -1) const;

    /**
     * Creates an AudioBuffer containing the data from a specified source, and with the same numbers of channels and samples as it.
     */
    static AudioBuffer from(const IAudioSampleProvider &src);

private:
    QVector<QVector<float>> m_buffer;
};



#endif // CHORUSKIT_AUDIOBUFFER_H
