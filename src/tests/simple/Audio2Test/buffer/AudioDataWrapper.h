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
    AudioDataWrapper(float *const *data, int channelCount, int sampleCount, int startPos = 0);

    float &sampleAt(int channel, int pos) override;
    float constSampleAt(int channel, int pos) const override;
    int channelCount() const override;
    int sampleCount() const override;

    float *const *data() const;
    void resetData(float *const *data, int channelCount, int sampleCount, int startPos = 0);
protected:
    QScopedPointer<AudioDataWrapperPrivate> d_ptr;
    AudioDataWrapper(AudioDataWrapperPrivate &d);
};



#endif // CHORUSKIT_AUDIODATAWRAPPER_H
