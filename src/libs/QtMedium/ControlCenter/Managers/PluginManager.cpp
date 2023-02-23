#include "PluginManager.h"
#include "private/PluginManager_p.h"

QMCC_BEGIN_NAMESPACE

PluginManager::PluginManager(QObject *parent) : PluginManager(*new PluginManagerPrivate(), parent) {
}

PluginManager::~PluginManager() {
}

PluginManager::PluginManager(PluginManagerPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

QMCC_END_NAMESPACE