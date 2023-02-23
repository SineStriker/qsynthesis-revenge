#ifndef QSPLUGINMANAGERPRIVATE_H
#define QSPLUGINMANAGERPRIVATE_H

#include "../QsPluginManager.h"
#include "QsAbstractManager_p.h"

#include <QPluginLoader>

class QSBASE_API QsPluginManagerPrivate : public QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsPluginManager)
public:
    QsPluginManagerPrivate();
    ~QsPluginManagerPrivate();

    void init();

    struct PluginSetInfo : public QsPluginManager::PluginSetOptions {
        QSharedPointer<QsPluginDir> d_ptr;
    };

    QHash<QString, PluginSetInfo> pluginSets;
};

#endif // QSPLUGINMANAGERPRIVATE_H
