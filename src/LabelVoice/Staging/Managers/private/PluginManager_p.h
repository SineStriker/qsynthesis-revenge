#ifndef PLUGINMANAGERPRIVATE_H
#define PLUGINMANAGERPRIVATE_H

#include "../PluginManager.h"
#include "Basic/BasicManager_p.h"

#include "Api/ISVSConverter.h"

#include <QPluginLoader>

class PluginManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(PluginManager)
public:
    PluginManagerPrivate();
    ~PluginManagerPrivate();

    void init();

    void loadConverters();
    void unloadConverters();

    struct ConverterInfo {
        QPluginLoader *loader;
        QString target;
        QStringList filters;
    };
    QList<ConverterInfo> converters;
    QMap<QString, int> converterMap;
};

#endif // PLUGINMANAGERPRIVATE_H
