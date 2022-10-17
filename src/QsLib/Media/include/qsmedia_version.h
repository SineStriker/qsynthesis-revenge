#ifndef QSMEDIA_VERSION_H
#define QSMEDIA_VERSION_H

#include "qsmedia_global.h"

#include <QList>

namespace FF {

    struct FFmpegLibInfo {
        QString name;
        quint32 version[3];
        QString license;
        QString configuration;
    };

    struct SDLInfo {
        quint32 version[3];
    };

    QSMEDIA_API QList<FFmpegLibInfo> GetFFmpegInfo();

    QSMEDIA_API SDLInfo GetSDLInfo();

} // namespace FF

#endif // QSMEDIA_VERSION_H
