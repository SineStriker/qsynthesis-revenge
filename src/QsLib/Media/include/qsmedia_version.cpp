#include "qsmedia_version.h"

extern "C" {
#include <SDL.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

static void FFmpegUnpackVersion(quint32 ver, quint32 *out) {
    out[0] = AV_VERSION_MAJOR(ver);
    out[1] = AV_VERSION_MINOR(ver);
    out[2] = AV_VERSION_MICRO(ver);
    return;
}

QList<FF::FFmpegLibInfo> FF::GetFFmpegInfo() {
    QList<FFmpegLibInfo> res;

#define FFMPEG_GET_INFO(LIB, OUT)                                                                  \
    {                                                                                              \
        quint32 ver = LIB##_version();                                                             \
        FFmpegUnpackVersion(ver, OUT.version);                                                     \
        OUT.name = #LIB;                                                                           \
        OUT.license = LIB##_license();                                                             \
        OUT.configuration = LIB##_configuration();                                                 \
    }

    FFmpegLibInfo info;
    FFMPEG_GET_INFO(avcodec, info);
    res.append(info);
    FFMPEG_GET_INFO(avformat, info);
    res.append(info);
    FFMPEG_GET_INFO(avutil, info);
    res.append(info);
    FFMPEG_GET_INFO(swresample, info);
    res.append(info);

#undef FFMPEG_GET_INFO

    return res;
}

FF::SDLInfo FF::GetSDLInfo() {
    struct SDL_version ver;
    SDL_GetVersion(&ver);
    return SDLInfo{{ver.major, ver.minor, ver.patch}};
}
