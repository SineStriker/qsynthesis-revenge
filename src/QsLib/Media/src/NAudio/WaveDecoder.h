#ifndef WAVEDECODER_H
#define WAVEDECODER_H

#include "WaveStream.h"

class QSMEDIA_API WaveDecoder : public WaveStream {
public:
    struct QSMEDIA_API WaveArguments {
        int SampleRate;
        int SampleFormat;
        int Channels;

        WaveArguments() : WaveArguments(-1, 0, -1) {
        }

        WaveArguments(int fmt) : WaveArguments(-1, fmt, -1) {
        }

        WaveArguments(int sampleRate, int fmt, int channels) {
            SampleRate = sampleRate;
            SampleFormat = fmt;
            Channels = channels;
        }
    };

    WaveDecoder() {
    }

    WaveDecoder(const WaveArguments &args) : _arguments(args) {
    }

    ~WaveDecoder();

    WaveArguments arguments() const;
    void setArguments(const WaveArguments &arguments);

protected:
    WaveArguments _arguments;
};

#endif // WAVEDECODER_H
