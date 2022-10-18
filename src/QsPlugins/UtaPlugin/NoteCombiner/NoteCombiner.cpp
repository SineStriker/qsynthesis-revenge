#include "NoteCombiner.h"

NoteCombiner::NoteCombiner(QObject *parent) : IUtaPlugin(parent) {
}

NoteCombiner::~NoteCombiner() {
}

int NoteCombiner::exec(IUtaPlugin::PluginArguments *args, void *userdata) {
    return 0;
}
