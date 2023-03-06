#ifndef SDLPLAYBACKPLUGIN_H
#define SDLPLAYBACKPLUGIN_H

#include "interfaces/IAudioPlaybackPlugin.h"

QSAPI_USING_NAMESPACE

class Q_DECL_EXPORT SDLPlaybackPlugin : public IAudioPlaybackPlugin {
    Q_OBJECT
    Q_INTERFACES(QsApi::IAudioPlaybackPlugin)
    Q_PLUGIN_METADATA(IID QsApi_IAudioPlaybackPlugin_IID FILE "plugin.json")
public:
    explicit SDLPlaybackPlugin(QObject *parent = nullptr);
    ~SDLPlaybackPlugin();

public:
    IAudioPlayback *create(const QString &key, QObject *parent) override;
};

#endif // SDLPLAYBACKPLUGIN_H
