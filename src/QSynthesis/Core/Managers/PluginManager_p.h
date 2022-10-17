#ifndef PLUGINMANAGERPRIVATE_H
#define PLUGINMANAGERPRIVATE_H

#include "Basic/BasicManager_p.h"

#include "PluginManager.h"

class PluginManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(PluginManager)
public:
    PluginManagerPrivate();
    ~PluginManagerPrivate();

    void init();
};

#endif // PLUGINMANAGERPRIVATE_H
