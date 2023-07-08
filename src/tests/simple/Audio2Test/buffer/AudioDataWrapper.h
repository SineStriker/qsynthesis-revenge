//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_AUDIODATAWRAPPER_H
#define CHORUSKIT_AUDIODATAWRAPPER_H

#include "IAudioSampleContainer.h"


class AudioDataWrapper: public IAudioSampleContainer {

public:
    AudioDataWrapper(float *const *data, int channelCount, int sampleCount, int startPos = 0);

    float &sampleAt(int channel, int pos) override;
    float constSampleAt(int channel, int pos) const override;
    int channelCount() const override;
    int sampleCount() const override;

    float *const *data() const;
    void resetData(float *const *data, int channelCount, int sampleCount, int startPos = 0);

private:
    float *const *m_data;
    int m_channelCount;
    int m_sampleCount;
    int m_startPos;
};



#endif // CHORUSKIT_AUDIODATAWRAPPER_H
