#include "FFmpegDecoderPlugin.h"

#include "FFmpegDecoder.h"

FFmpegDecoderPlugin::FFmpegDecoderPlugin(QObject *parent) : IAudioDecoderPlugin(parent) {
}

FFmpegDecoderPlugin::~FFmpegDecoderPlugin() {
}

IAudioDecoder *FFmpegDecoderPlugin::create(const QString &key, QObject *parent) {
    Q_UNUSED(key)
    return new FFmpegDecoder(parent);
}
