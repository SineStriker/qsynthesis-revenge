#ifndef WAVESTREAM_H
#define WAVESTREAM_H

#include <QIODevice>

#include <fstream>

#include "WaveFormat.h"

class QSMEDIA_API WaveStream {
public:
    WaveStream();
    ~WaveStream();

    virtual WaveFormat Format() const = 0;

    virtual void Flush();

    enum SeekOrigin {
        // These constants match Win32's FILE_BEGIN, FILE_CURRENT, and FILE_END
        Begin = 0,
        Current = 1,
        End = 2,
    };

    virtual void SetPosition(qint64 pos) = 0;

    virtual qint64 Position() const = 0;
    virtual qint64 Length() const = 0;

    qint64 Seek(long offset, SeekOrigin origin);

    virtual int Read(char *buffer, int offset, int count) = 0;

    // Inherit from ISampleProvider
    virtual int Read(float *buffer, int offset, int count);

    virtual int BlockAlign() const;

    void Skip(int seconds);

    double CurrentTime() const;
    void SetCurrentTime(double value);

    virtual double TotalTime() const;

    virtual bool HasData(int count) const;
};

#endif // WAVESTREAM_H
