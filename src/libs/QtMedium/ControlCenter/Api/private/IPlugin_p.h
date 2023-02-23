#ifndef IPLUGIN_P_H
#define IPLUGIN_P_H

#include "../IPlugin.h"

QMCC_BEGIN_NAMESPACE

class IPluginPrivate {
    Q_DECLARE_PUBLIC(IPlugin)
public:
    IPluginPrivate();
    virtual ~IPluginPrivate();

    void init();

    IPlugin *q_ptr;
};

QMCC_END_NAMESPACE

#endif // IPLUGIN_P_H
