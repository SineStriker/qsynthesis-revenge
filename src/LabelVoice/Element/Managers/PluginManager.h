#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "Api/ISVSConverter.h"
#include "Basic/BasicManager.h"

#include "qsutils_macros.h"

#include <QPluginLoader>

class PluginManagerPrivate;

#include "lvelem_global.h"

class LVELEM_API PluginManager : public BasicManager {
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

    enum InternalPlugins {
        ZipLib,
        NativeWindow,
    };

    static QPluginLoader *loadInternalPlugin(InternalPlugins id);

protected:
    PluginManager(PluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // PLUGINMANAGER_H
