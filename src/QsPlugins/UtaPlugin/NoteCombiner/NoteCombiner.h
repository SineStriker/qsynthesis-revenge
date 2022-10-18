#ifndef NOTECOMBINER_H
#define NOTECOMBINER_H

#include "Api/IUtaPlugin.h"

class Q_DECL_EXPORT NoteCombiner : public IUtaPlugin {
    Q_OBJECT
    Q_INTERFACES(IUtaPlugin)
    Q_PLUGIN_METADATA(IID IUtaPlugin_IID FILE "plugin.json")
public:
    explicit NoteCombiner(QObject *parent = nullptr);
    ~NoteCombiner();

    int exec(PluginArguments *args, void *userdata = nullptr) override;
};

#endif // NOTECOMBINER_H
