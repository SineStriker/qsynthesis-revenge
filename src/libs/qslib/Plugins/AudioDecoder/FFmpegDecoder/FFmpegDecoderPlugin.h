#ifndef FFMPEGDECODERPLUGIN_H
#define FFMPEGDECODERPLUGIN_H

#include "interfaces/IAudioDecoderPlugin.h"

QSAPI_USING_NAMESPACE

class Q_DECL_EXPORT FFmpegDecoderPlugin : public IAudioDecoderPlugin {
    Q_OBJECT
    Q_INTERFACES(QsApi::IAudioDecoderPlugin)
    Q_PLUGIN_METADATA(IID QsApi_IAudioDecoderPlugin_IID FILE "plugin.json")
public:
    explicit FFmpegDecoderPlugin(QObject *parent = nullptr);
    ~FFmpegDecoderPlugin();

public:
    IAudioDecoder *create(const QString &key, QObject *parent) override;
};

#endif // FFMPEGDECODERPLUGIN_H
