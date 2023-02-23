#ifndef PLUGINMANAGERPRIVATE_H
#define PLUGINMANAGERPRIVATE_H

#include "../PluginManager.h"

QMCC_BEGIN_NAMESPACE

class PluginManagerPrivate {
    Q_DECLARE_PUBLIC(PluginManager)
public:
    PluginManagerPrivate();
    virtual ~PluginManagerPrivate();

    void init();

    PluginManager *q_ptr;
};

QMCC_END_NAMESPACE

#endif // PLUGINMANAGERPRIVATE_H