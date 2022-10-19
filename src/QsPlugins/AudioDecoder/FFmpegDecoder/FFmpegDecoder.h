#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "Api/IAudioDecoder.h"

class Q_DECL_EXPORT FFmpegDecoder : public IAudioDecoder {
    Q_OBJECT
    Q_INTERFACES(IAudioDecoder)
    Q_PLUGIN_METADATA(IID IAudioDecoder_IID FILE "plugin.json")
public:
    explicit FFmpegDecoder(QObject *parent = nullptr);
    ~FFmpegDecoder();

signals:
};

#endif // FFMPEGDECODER_H
