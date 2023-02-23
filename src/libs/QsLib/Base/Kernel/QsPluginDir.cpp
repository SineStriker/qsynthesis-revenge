#include "QsPluginDir.h"
#include "private/QsPluginDir_p.h"

QsPluginDir::QsPluginDir(QObject *parent) : QsPluginDir(*new QsPluginDirPrivate(), parent) {
}

QsPluginDir::~QsPluginDir() {
}

void QsPluginDir::load(const QString &dir) {
    Q_D(QsPluginDir);
    if (hasPlugin()) {
        unload();
    }
    d->loadPlugins(dir);
}

void QsPluginDir::unload() {
    Q_D(QsPluginDir);
    d->unloadPlugins();
}

bool QsPluginDir::hasPlugin() const {
    Q_D(const QsPluginDir);
    return !(d->priorPlugins.empty() && d->plugins.isEmpty());
}

QObjectList QsPluginDir::instances() const {
    Q_D(const QsPluginDir);
    QObjectList res;
    for (const auto &pp : d->priorPlugins) {
        auto loader = pp.loader;
        res.append(loader->instance());
    }
    for (const auto &loader : d->plugins) {
        res.append(loader->instance());
    }
    return res;
}

QsPluginDir::QsPluginDir(QsPluginDirPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
