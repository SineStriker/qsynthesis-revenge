#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "Basic/BasicManager.h"

#include "qsutils_macros.h"

class PluginManagerPrivate;

class PluginManager : public BasicManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PluginManager)
    Q_SINGLETON(PluginManager)
public:
    PluginManager(QObject *parent = nullptr);
    ~PluginManager();

protected:
    PluginManager(PluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // PLUGINMANAGER_H
