#ifndef __QSMEDIANAMESPACE_H__
#define __QSMEDIANAMESPACE_H__

#include "QsMediaGlobal.h"

QSMEDIA_BEGIN_NAMESPACE

const char KEY_NAME_FILE_NAME[] = "FileName";

const char KEY_NAME_SAMPLE_RATE[] = "SampleRate";

const char KEY_NAME_SAMPLE_FORMAT[] = "SampleFormat";

const char KEY_NAME_CHANNELS[] = "Channels";

enum FFmpeg_AVSampleFormat {
    AV_SAMPLE_FMT_NONE = -1, // 0xFFFFFFFF
    /// <summary>unsigned 8 bits</summary>
    AV_SAMPLE_FMT_U8 = 0,
    /// <summary>signed 16 bits</summary>
    AV_SAMPLE_FMT_S16 = 1,
    /// <summary>signed 32 bits</summary>
    AV_SAMPLE_FMT_S32 = 2,
    /// <summary>float</summary>
    AV_SAMPLE_FMT_FLT = 3,
    /// <summary>double</summary>
    AV_SAMPLE_FMT_DBL = 4,
    /// <summary>unsigned 8 bits, planar</summary>
    AV_SAMPLE_FMT_U8P = 5,
    /// <summary>signed 16 bits, planar</summary>
    AV_SAMPLE_FMT_S16P = 6,
    /// <summary>signed 32 bits, planar</summary>
    AV_SAMPLE_FMT_S32P = 7,
    /// <summary>float, planar</summary>
    AV_SAMPLE_FMT_FLTP = 8,
    /// <summary>double, planar</summary>
    AV_SAMPLE_FMT_DBLP = 9,
    /// <summary>signed 64 bits</summary>
    AV_SAMPLE_FMT_S64 = 10, // 0x0000000A
    /// <summary>signed 64 bits, planar</summary>
    AV_SAMPLE_FMT_S64P = 11, // 0x0000000B
    /// <summary>Number of sample formats. DO NOT USE if linking dynamically</summary>
    AV_SAMPLE_FMT_NB = 12, // 0x0000000C
};

QSMEDIA_END_NAMESPACE

#endif // __QSMEDIANAMESPACE_H__