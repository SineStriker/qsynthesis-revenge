#ifndef FFMPEGENCODERPLUGIN_H
#define FFMPEGENCODERPLUGIN_H

#include "Api/interfaces/IAudioEncoderPlugin.h"

QSAPI_USING_NAMESPACE

class Q_DECL_EXPORT FFmpegEncoderPlugin : public IAudioEncoderPlugin {
    Q_OBJECT
    Q_INTERFACES(QsApi::IAudioEncoderPlugin)
    Q_PLUGIN_METADATA(IID QsApi_IAudioEncoderPlugin_IID FILE "plugin.json")
public:
    explicit FFmpegEncoderPlugin(QObject *parent = nullptr);
    ~FFmpegEncoderPlugin();

public:
    IAudioEncoder *create(const QString &key, QObject *parent) override;
};

#endif // FFMPEGENCODERPLUGIN_H
