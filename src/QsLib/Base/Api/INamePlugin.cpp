#include "INamePlugin.h"

#include <QHash>
#include <QThread>

static QHash<QThread *, QString> lastPluginPaths;

INamePlugin::INamePlugin(QObject *parent) : QObject(parent) {
    auto it = lastPluginPaths.find(QThread::currentThread());
    if (it != lastPluginPaths.end()) {
        this->path = it.value();
        lastPluginPaths.erase(it);
    }
}

INamePlugin::~INamePlugin() {
}

void INamePlugin::setLastPluginPath(const QString &path) {
    lastPluginPaths[QThread::currentThread()] = path;
}
