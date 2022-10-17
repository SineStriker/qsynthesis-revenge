#ifndef FFMPEGCONVERTER_H
#define FFMPEGCONVERTER_H

#include <QString>

#include "qsmedia_global.h"

class QSMEDIA_API FFmpegConverter {
public:
    FFmpegConverter();
    ~FFmpegConverter();

    struct PCMArgs {
        FF::SampleFormat fmt;
        int sampleRate;
    };

    struct PCMResult {
        std::string bytes;
        int channels;
    };

    int convert(const QString &filename, const PCMArgs &args, PCMResult *pcm_pack);
};

#endif // FFMPEGCONVERTER_H
