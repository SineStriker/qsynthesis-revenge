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
        Synchronous,
    };
    virtual quint64 read(AudioBufferList &buf, quint64 size) = 0; //TODO return value < 0 for error
    virtual quint64 peek(AudioBufferList &buf, quint64 size, quint64 offset) = 0;
    virtual bool setReadMode(ReadMode mode);
    virtual ReadMode readMode();
    virtual quint64 pos();
    virtual bool setPos(quint64 pos);
    virtual bool isSequential() = 0;
    virtual quint32 sampleRate() = 0;
    virtual bool setSampleRate(quint32 sampleRate);
    virtual bool isSampleRateChangeable() = 0;
    virtual quint16 channelCount() = 0;
    virtual quint64 readableSampleCount() = 0;
    inline double msec() {
        return 1000.0 * pos() / sampleRate();
    }
    inline bool setMsec(double msec) {
        return setPos(msec * sampleRate() / 1000);
    }
    virtual bool open();
    virtual void close();
    QString lastError();
protected:
    void setLastError(const QString &err);
private:
    QString m_err;
};



#endif // CHORUSKIT_IAUDIOSOURCE_H
