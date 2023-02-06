#include "INamePlugin.h"

#include <QHash>
#include <QMutex>
#include <QThread>

static QHash<QThread *, QString> lastPluginPaths;

static QMutex mutex;

INamePlugin::INamePlugin(QObject *parent) : QObject(parent) {
    QMutexLocker locker(&mutex);
    auto it = lastPluginPaths.find(QThread::currentThread());
    if (it != lastPluginPaths.end()) {
        this->path = it.value();
        lastPluginPaths.erase(it);
    }
}

INamePlugin::~INamePlugin() {
}

void INamePlugin::setLastPluginPath(const QString &path) {
    QMutexLocker locker(&mutex);
    lastPluginPaths[QThread::currentThread()] = path;
}
