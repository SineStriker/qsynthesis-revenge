#ifndef CHORUSKIT_AUDIOBUFFER_H
#define CHORUSKIT_AUDIOBUFFER_H

#include "IAudioSampleContainer.h"
#include <vector>


class AudioBuffer: public IAudioSampleContainer {

public:

    AudioBuffer();
    AudioBuffer(int channelCount, qint64 sampleCount);

    float &sampleAt(int channel, qint64 pos) override;
    float constSampleAt(int channel, qint64 pos) const override;

    int channelCount() const override;
    qint64 sampleCount() const override;

    void resize(int newChannelCount = -1, qint64 newSampleCount = -1);
    float *data(int channel);
    float const *constData(int channel) const;

    std::vector<float> &vector(int channel);
    const std::vector<float> &constVector(int channel) const;

    AudioBuffer slice(int startChannelIndex, qint64 startSampleCount, int channelSize = -1, qint64 length = -1) const;

    static AudioBuffer from(const IAudioSampleProvider &src);

private:
    std::vector<std::vector<float>> m_buffer;
};



#endif // CHORUSKIT_AUDIOBUFFER_H
