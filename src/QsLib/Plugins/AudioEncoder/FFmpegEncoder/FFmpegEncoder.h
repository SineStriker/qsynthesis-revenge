#ifndef FFMPEGENCODER_H
#define FFMPEGENCODER_H

#include "Api/IAudioEncoder.h"

QSAPI_USING_NAMESPACE

class Q_DECL_EXPORT FFmpegEncoder : public IAudioEncoder {
    Q_OBJECT
    Q_INTERFACES(QsApi::IAudioEncoder)
    Q_PLUGIN_METADATA(IID QsApi_IAudioEncoder_IID FILE "plugin.json")
public:
    explicit FFmpegEncoder(QObject *parent = nullptr);
    ~FFmpegEncoder();

signals:
};

#endif // FFMPEGENCODER_H
