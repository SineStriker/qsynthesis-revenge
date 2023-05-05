//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_IAUDIOSOURCE_H
#define CHORUSKIT_IAUDIOSOURCE_H

#include "AudioBuffer.h"

class IAudioSource {
public:
    enum ReadMode {
        Immediate,
        FillWithEmpty,
        Synchronous,
    };
    virtual quint64 read(AudioBuffer *buf, quint64 size, ReadMode mode) = 0;
    virtual quint64 peek(AudioBuffer *buf, quint64 size, ReadMode mode) = 0;
    virtual quint64 pos() = 0;
    virtual bool setPos(quint64 pos) = 0;
    virtual bool isSequential() = 0;
    virtual quint32 sampleRate() = 0;
    virtual bool setSampleRate(quint32 sampleRate) = 0;
    virtual bool isSampleRateChangeable() = 0;
    virtual quint16 channelCount() = 0;
    virtual quint64 readableSampleCount() = 0;
    virtual bool canRead(quint64 size) = 0;
    inline double msec() {
        return 1000.0 * pos() / sampleRate();
    }
    inline bool setMsec(double msec) {
        return setPos(msec * sampleRate() / pos() / 1000);
    }
};



#endif // CHORUSKIT_IAUDIOSOURCE_H
