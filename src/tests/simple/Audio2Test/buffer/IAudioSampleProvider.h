//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_IAUDIOSAMPLEPROVIDER_H
#define CHORUSKIT_IAUDIOSAMPLEPROVIDER_H

#include <qglobal.h>

class IAudioSampleProvider {
public:
    virtual ~IAudioSampleProvider() = default;
    virtual float constSampleAt(int channel, qint64 pos) const = 0;
    virtual int channelCount() const = 0;
    virtual qint64 sampleCount() const = 0;
    float magnitude(int channel, qint64 startPos, qint64 length) const;
    float magnitude(int channel) const;
    float rms(int channel, qint64 startPos, qint64 length) const;
    float rms(int channel) const;
};



#endif // CHORUSKIT_IAUDIOSAMPLEPROVIDER_H
