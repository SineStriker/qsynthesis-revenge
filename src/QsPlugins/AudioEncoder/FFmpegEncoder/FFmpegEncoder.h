#ifndef FFMPEGENCODER_H
#define FFMPEGENCODER_H

#include "Api/IAudioEncoder.h"

class Q_DECL_EXPORT FFmpegEncoder : public IAudioEncoder {
    Q_OBJECT
    Q_INTERFACES(IAudioEncoder)
    Q_PLUGIN_METADATA(IID IAudioEncoder_IID FILE "plugin.json")
public:
    explicit FFmpegEncoder(QObject *parent = nullptr);
    ~FFmpegEncoder();

signals:
};

#endif // FFMPEGENCODER_H
