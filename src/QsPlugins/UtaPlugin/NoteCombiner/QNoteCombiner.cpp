#include "QNoteCombiner.h"

QNoteCombiner::QNoteCombiner(QObject *parent) : IUtaPlugin(parent) {
}

QNoteCombiner::~QNoteCombiner() {
}

int QNoteCombiner::exec(IUtaPlugin::PluginArguments *args, void *userdata) {
    return 0;
}
