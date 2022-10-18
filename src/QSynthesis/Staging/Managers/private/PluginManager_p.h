#ifndef PLUGINMANAGERPRIVATE_H
#define PLUGINMANAGERPRIVATE_H

#include "../PluginManager.h"
#include "Basic/BasicManager_p.h"

class PluginManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(PluginManager)
public:
    PluginManagerPrivate();
    ~PluginManagerPrivate();

    void init();
};

#endif // PLUGINMANAGERPRIVATE_H
