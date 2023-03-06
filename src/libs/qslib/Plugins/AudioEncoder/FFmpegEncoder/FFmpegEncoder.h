#ifndef FFMPEGENCODER_H
#define FFMPEGENCODER_H

#include "Api/IAudioEncoder.h"

QSAPI_USING_NAMESPACE

class Q_DECL_EXPORT FFmpegEncoder : public IAudioEncoder {
    Q_OBJECT
public:
    explicit FFmpegEncoder(QObject *parent = nullptr);
    ~FFmpegEncoder();

signals:
};

#endif // FFMPEGENCODER_H
