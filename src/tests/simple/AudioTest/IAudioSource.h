//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_IAUDIOSOURCE_H
#define CHORUSKIT_IAUDIOSOURCE_H

#include "AudioBuffer.h"

class AudioBus;

class IAudioSource {
public:
    enum ReadMode {
        Immediate,
        Synchronous,
    };
    virtual quint64 read(AudioBufferList &buf, quint64 size) = 0; //TODO return value < 0 for error
    virtual quint64 peek(AudioBufferList &buf, quint64 size, quint64 offset) = 0;
    virtual bool setReadMode(ReadMode mode);
    virtual ReadMode readMode() const;
    virtual quint64 pos() const;
    virtual bool setPos(quint64 pos);
    virtual bool isSequential() const = 0;
    virtual quint32 sampleRate() const = 0;
    virtual bool setSampleRate(quint32 sampleRate);
    virtual bool isSampleRateChangeable() const = 0;
    virtual quint16 channelCount() const = 0;
    virtual quint64 readableSampleCount() const = 0;
    inline double msec() const {
        return 1000.0 * pos() / sampleRate();
    }
    inline bool setMsec(double msec) {
        return setPos(msec * sampleRate() / 1000);
    }
    virtual bool open();
    virtual void close();
    QString lastError() const;
protected:
    void setLastError(const QString &err);
private:
    friend class AudioBus;
    AudioBus *m_bus = nullptr;
    QString m_err;
};



#endif // CHORUSKIT_IAUDIOSOURCE_H
