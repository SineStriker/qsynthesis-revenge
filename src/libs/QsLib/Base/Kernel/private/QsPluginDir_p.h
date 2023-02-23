#ifndef QSPLUGINDIRPRIVATE_H
#define QSPLUGINDIRPRIVATE_H

#include "../QsPluginDir.h"

#include <QPluginLoader>

#include <set>

struct PriorPlugin {
    int priority;
    QPluginLoader *loader;
    PriorPlugin() : priority(0), loader(nullptr){};
};

struct plugin_less {
    bool operator()(const PriorPlugin &a, const PriorPlugin &b) const {
        return a.priority < b.priority;
    }
};

class QsPluginDirPrivate {
    Q_DECLARE_PUBLIC(QsPluginDir)
public:
    QsPluginDirPrivate();
    virtual ~QsPluginDirPrivate();

    void init();

    QsPluginDir *q_ptr;

    void loadPlugins(const QString &dirname);
    void unloadPlugins();

    std::set<PriorPlugin, plugin_less> priorPlugins;
    QList<QPluginLoader *> plugins;
};

#endif // QSPLUGINDIRPRIVATE_H
