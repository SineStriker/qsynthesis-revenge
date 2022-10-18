#include "QNoteCombiner.h"

QNoteCombiner::QNoteCombiner(QObject *parent) : QObject(parent) {
}

QNoteCombiner::~QNoteCombiner() {
}

int QNoteCombiner::exec(IUtaPlugin::PluginArguments *args) {
    return 0;
}
