#include "IPlugin.h"
#include "private/IPlugin_p.h"

QMCC_BEGIN_NAMESPACE

IPlugin::IPlugin(QObject *parent) : IPlugin(*new IPluginPrivate(), parent) {
}

IPlugin::~IPlugin() {
}

IPlugin::IPlugin(IPluginPrivate &d, QObject *parent) : QMNamePlugin(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

QMCC_END_NAMESPACE