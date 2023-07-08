#ifndef CHORUSKIT_AUDIOBUFFER_H
#define CHORUSKIT_AUDIOBUFFER_H

#include "IAudioSampleContainer.h"
#include <QVector>


class AudioBuffer: public IAudioSampleContainer {

public:

    AudioBuffer();
    AudioBuffer(int channelCount, int sampleCount);

    float &sampleAt(int channel, int pos) override;
    float constSampleAt(int channel, int pos) const override;

    int channelCount() const override;
    int sampleCount() const override;

    void resize(int newChannelCount = -1, int newSampleCount = -1);
    float *data(int channel);
    float const *constData(int channel) const;

    QVector<float> &vector(int channel);
    const QVector<float> &constVector(int channel) const;

    AudioBuffer slice(int startChannelIndex, int startSampleCount, int channelSize = -1, int length = -1) const;

private:
    QVector<QVector<float>> m_buffer;
};



#endif // CHORUSKIT_AUDIOBUFFER_H
