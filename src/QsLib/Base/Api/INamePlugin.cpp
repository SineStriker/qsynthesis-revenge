#include "INamePlugin.h"

INamePlugin::INamePlugin(QObject *parent) : QObject(parent) {
}

INamePlugin::~INamePlugin() {
}

void INamePlugin::setupPath(const QString &path) {
    this->path = path;
}
