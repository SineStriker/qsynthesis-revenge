#include "PluginManager.h"
#include "PluginManager_p.h"

Q_SINGLETON_DECLARE(PluginManager)

PluginManager::PluginManager(QObject *parent) : PluginManager(*new PluginManagerPrivate(), parent) {
}

PluginManager::~PluginManager() {
}

PluginManager::PluginManager(PluginManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();
    d.init();
}
