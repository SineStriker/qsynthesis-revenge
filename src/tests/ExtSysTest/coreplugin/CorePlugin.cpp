#include "CorePlugin.h"

Core::Internal::CorePlugin::CorePlugin() {
}

Core::Internal::CorePlugin::~CorePlugin() {
}

bool Core::Internal::CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
    qDebug() << "initialize Core Plugin";
    return true;
}

void Core::Internal::CorePlugin::extensionsInitialized() {
    qDebug() << "extensions initialized Core Plugin";
}

bool Core::Internal::CorePlugin::delayedInitialize() {
    qDebug() << "delayed initialize Core Plugin";
    return true;
}
