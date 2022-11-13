#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "Api/ISVSConverter.h"
#include "Basic/BasicManager.h"
#include "Kernel/QsDistConfig.h"

#include <QPluginLoader>

#include "qsintegrate_global.h"
#include "qsutils_macros.h"

class PluginManagerPrivate;

class QSINTEGRATE_API PluginManager : public BasicManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PluginManager)
    Q_SINGLETON(PluginManager)
public:
    PluginManager(QObject *parent = nullptr);
    ~PluginManager();

public:
    void load();
    void save();

    QString converterFilters() const;
    ISVSConverter *searchConverter(const QString &suffix) const;

    static QPluginLoader *loadInternalPlugin(QsDistConfig::InternalPlugins id);

protected:
    PluginManager(PluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // PLUGINMANAGER_H
