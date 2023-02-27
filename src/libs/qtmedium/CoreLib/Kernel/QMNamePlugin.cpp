#include "QMNamePlugin.h"

#include <QHash>
#include <QMessageLogger>
#include <QMutex>
#include <QThread>

struct QMNamePluginData {
    QHash<QThread *, QString> lastPluginPaths;
    QMutex mutex;
};

Q_GLOBAL_STATIC(QMNamePluginData, global);

QMNamePlugin::QMNamePlugin(QObject *parent) : QObject(parent) {
    QMutexLocker locker(&global->mutex);
    auto it = global->lastPluginPaths.find(QThread::currentThread());
    if (it != global->lastPluginPaths.end()) {
        this->path = it.value();
        global->lastPluginPaths.erase(it);
    }
}

QMNamePlugin::~QMNamePlugin() {
}

void QMNamePlugin::setLastPluginPath(const QString &path) {
    QMutexLocker locker(&global->mutex);
    global->lastPluginPaths[QThread::currentThread()] = path;
}
