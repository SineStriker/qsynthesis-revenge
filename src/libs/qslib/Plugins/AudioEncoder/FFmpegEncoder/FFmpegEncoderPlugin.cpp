#include "FFmpegEncoderPlugin.h"

#include "FFmpegEncoder.h"

FFmpegEncoderPlugin::FFmpegEncoderPlugin(QObject *parent) : IAudioEncoderPlugin(parent) {
}

FFmpegEncoderPlugin::~FFmpegEncoderPlugin() {
}

IAudioEncoder *FFmpegEncoderPlugin::create(const QString &key, QObject *parent) {
    Q_UNUSED(key)
    return new FFmpegEncoder(parent);
}
